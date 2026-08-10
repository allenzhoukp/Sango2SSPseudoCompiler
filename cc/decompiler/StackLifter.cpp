#include "Decompiler.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <limits>
#include <queue>
#include <sstream>

namespace sango {
namespace decompiler {

namespace {

/** Returns a compact decimal spelling for a signed object operand. */
std::string integerText(std::int32_t value) {
    return std::to_string(value);
}

/** Returns a round-trippable spelling for standard single-precision float bits. */
std::string floatingPointText(std::int32_t bits) {
    float value = 0.0f;
    std::uint32_t unsignedBits = static_cast<std::uint32_t>(bits);
    std::memcpy(&value, &unsignedBits, sizeof(value));
    if (!std::isfinite(value)) {
        return integerText(bits);
    }
    std::ostringstream output;
    output << std::setprecision(9) << value;
    std::string text = output.str();
    if (text.find('.') == std::string::npos
            && text.find('e') == std::string::npos
            && text.find('E') == std::string::npos) {
        text += ".0";
    }
    return text;
}

/** Returns the function-table index for a direct call target. */
int findFunction(const Module& module, std::uint32_t address) {
    for (std::size_t index = 0; index < module.functions.size(); ++index) {
        if (module.functions[index].entryAddress == address) {
            return static_cast<int>(index);
        }
    }
    return -1;
}

/** Returns a source name for one parameter or local stack slot. */
std::string stackSlotName(const Module& module, int functionIndex,
                          int slot, int localBase) {
    if (slot <= -2) {
        const int parameter = module.functions[functionIndex].parameterCount + slot + 1;
        return parameter >= 0 ? "a" + std::to_string(parameter)
                              : "invalid_argument";
    }
    return "v" + std::to_string(slot - localBase + 1);
}

/** Detects whether an object uses legacy slot one or current slot three for locals. */
int localSlotBase(const FunctionGraph& graph) {
    int smallest = std::numeric_limits<int>::max();
    for (std::size_t block = 0; block < graph.blocks.size(); ++block) {
        for (std::size_t index = 0;
                index < graph.blocks[block].instructions.size(); ++index) {
            const Instruction& instruction
                = *graph.blocks[block].instructions[index];
            const bool directLocal = instruction.opcode == 0x08
                || instruction.opcode == 0x0b
                || instruction.opcode == 0x1d
                || instruction.opcode == 0x20
                || instruction.opcode == 0x4e
                || instruction.opcode == 0x51
                || instruction.opcode == 0x54
                || instruction.opcode == 0x57;
            if (directLocal && instruction.operand > 0) {
                smallest = std::min(smallest, instruction.operand);
            }
        }
    }
    return smallest <= 2 ? 1 : 3;
}

/** Returns a configured engine-value name or a numeric intrinsic read. */
std::string internalValueRead(const ConfigurationNames& names, int index) {
    std::unordered_map<std::int32_t, std::string>::const_iterator found
        = names.internalValues.find(index);
    return found == names.internalValues.end()
        ? "GetIntv(" + std::to_string(index) + ")" : found->second;
}

/** Returns an indexed internal-value address without duplicating its expression. */
std::string indexedInternalValue(int base, const Expression& index) {
    return base == 0 ? index.text
        : "(" + index.text + " + " + std::to_string(base) + ")";
}

/** Finds the original bytes represented by a previously quoted object string. */
std::string unquotedObjectString(const Module& module,
                                 const std::string& quoted) {
    for (std::map<std::uint32_t, ObjectString>::const_iterator
            found = module.strings.begin(); found != module.strings.end(); ++found) {
        if (quoteSourceString(found->second.bytes) == quoted) {
            return found->second.bytes;
        }
    }
    return std::string();
}

/** Records one external asynchronous target and its inferred argument types. */
void rememberExternalFunction(LiftedFunction& function,
                              const std::string& name,
                              const std::vector<Expression>& arguments) {
    std::vector<ValueType>& types = function.externalFunctions[name];
    if (types.empty()) {
        for (std::size_t index = 0; index < arguments.size(); ++index) {
            types.push_back(arguments[index].type);
        }
    }
}

/** Formats one value-producing instruction for reuse in a local __asm block. */
std::string expressionAssemblyInstruction(const Module& module,
                                          int functionIndex,
                                          int localSlotAdjustment,
                                          const Instruction& instruction) {
    const InstructionDescription* description
        = instructionDescription(instruction.opcode);
    if (description == NULL) {
        return std::string();
    }
    std::ostringstream output;
    output << ' ' << description->name;
    if (instruction.operandCount >= 1) {
        output << ' ';
        if (instruction.opcode == 0x04) {
            const int target = findFunction(
                module, static_cast<std::uint32_t>(instruction.operand));
            if (target < 0) {
                return std::string();
            }
            output << sourceIdentifier(module.functions[target].name, target);
        } else if (instruction.opcode == 0x07) {
            if (instruction.operand < 0
                    || static_cast<std::size_t>(instruction.operand)
                        >= module.runtimeStringAddresses.size()) {
                return std::string();
            }
            const std::uint32_t address
                = module.runtimeStringAddresses[instruction.operand];
            std::map<std::uint32_t, ObjectString>::const_iterator found
                = module.strings.find(address);
            if (found == module.strings.end()) {
                return std::string();
            }
            output << quoteSourceString(found->second.bytes);
        } else if (instruction.opcode == 0x05) {
            output << "0x" << std::hex << std::uppercase
                   << static_cast<std::uint32_t>(instruction.operand)
                   << std::dec << std::nouppercase;
        } else {
            const bool directLocal = instruction.opcode == 0x08
                || instruction.opcode == 0x0b
                || instruction.opcode == 0x1d
                || instruction.opcode == 0x20
                || instruction.opcode == 0x4e
                || instruction.opcode == 0x51
                || instruction.opcode == 0x54
                || instruction.opcode == 0x57;
            output << (directLocal && instruction.operand > 0
                ? instruction.operand + localSlotAdjustment
                : instruction.operand);
        }
    }
    if (instruction.operandCount == 2) {
        output << ", ";
        if (instruction.opcode == 0x05) {
            const std::uint32_t packed
                = static_cast<std::uint32_t>(instruction.secondOperand);
            output << '(' << (packed & 0xffffU) << " | ("
                   << (packed >> 16) << " << 16))";
        } else {
            output << instruction.secondOperand;
        }
    }
    output << '\n';
    return output.str();
}

/** Joins argument provenance and the instruction that consumes those values. */
std::string expressionAssembly(const Module& module, int functionIndex,
                               int localSlotAdjustment,
                               const std::vector<Expression>& arguments,
                               const Instruction& instruction) {
    std::string result;
    for (std::size_t index = 0; index < arguments.size(); ++index) {
        if (arguments[index].assembly.empty()) {
            return std::string();
        }
        result += arguments[index].assembly;
    }
    const std::string operation
        = expressionAssemblyInstruction(
            module, functionIndex, localSlotAdjustment, instruction);
    return operation.empty() ? std::string() : result + operation;
}

/** Contains an arity-mismatched asynchronous call in one explained asm block. */
bool inlineAssemblyCallbs(const Module& module, int functionIndex,
                          int localSlotAdjustment,
                          const Instruction& instruction,
                          const std::vector<Expression>& values,
                          int target, std::string& statement) {
    const std::string assembly
        = expressionAssembly(module, functionIndex, localSlotAdjustment,
                             values, instruction);
    if (assembly.empty()) {
        return false;
    }
    std::ostringstream output;
    output << "// Inline assembly: CALLBS encodes 4 arguments, but "
           << sourceIdentifier(module.functions[target].name, target)
           << " declares " << module.functions[target].parameterCount << ".\n"
           << "__asm {\n" << assembly << '}';
    statement = output.str();
    return true;
}

/** Writes a simple source expression as one equivalent assembly push. */
bool writeAssemblyPush(std::ostream& output, const Expression& expression,
                       const ConfigurationNames& names) {
    if (expression.hasSideEffect || expression.text.empty()) {
        return false;
    }
    char* end = NULL;
    const long integer = std::strtol(expression.text.c_str(), &end, 10);
    if (end != expression.text.c_str() && *end == '\0') {
        output << " PUSH " << integer;
        return true;
    }
    if (expression.text[0] == '"') {
        output << " PUSHSTR " << expression.text;
        return true;
    }
    if ((expression.text[0] == 'a' || expression.text[0] == 'v')
            && expression.text.find_first_not_of("0123456789", 1)
                == std::string::npos) {
        output << " PUSHARG " << expression.text;
        return true;
    }
    if (expression.text.compare(0, 5, "g_var") == 0
            && expression.text.find_first_not_of("0123456789", 5)
                == std::string::npos) {
        output << " PUSHGLB " << expression.text.substr(5);
        return true;
    }
    for (std::unordered_map<std::int32_t, std::string>::const_iterator
            internal = names.internalValues.begin();
            internal != names.internalValues.end(); ++internal) {
        if (internal->second == expression.text) {
            output << " PUSHINV " << internal->first;
            return true;
        }
    }
    return false;
}

/** Formats one void system call as a small inline assembly statement. */
bool inlineAssemblySystemCall(const Instruction& instruction,
                              const std::vector<Expression>& arguments,
                              const ConfigurationNames& names,
                              std::string& statement) {
    std::ostringstream output;
    output << "__asm {\n";
    for (std::size_t index = 0; index < arguments.size(); ++index) {
        if (!writeAssemblyPush(output, arguments[index], names)) {
            return false;
        }
        output << '\n';
    }
    output << " SYSCALL 0x" << std::hex << std::uppercase
           << static_cast<std::uint32_t>(instruction.operand)
           << std::dec << std::nouppercase << ", ("
           << arguments.size() << " | (0 << 16))\n}";
    statement = output.str();
    return true;
}

/** Removes and returns the top expression, marking underflow on failure. */
Expression popExpression(std::vector<Expression>& stack, bool& underflow) {
    if (stack.empty()) {
        underflow = true;
        return Expression();
    }
    Expression result = stack.back();
    stack.pop_back();
    return result;
}

/** Wraps a unary operation so Sango's unusual precedence cannot change it. */
Expression unaryExpression(const std::string& operation,
                           const Expression& operand,
                           ValueType type) {
    Expression result;
    result.text = "(" + operation + operand.text + ")";
    result.type = type;
    result.hasSideEffect = operand.hasSideEffect;
    return result;
}

/** Wraps a binary operation and preserves its left-to-right value order. */
Expression binaryExpression(const Expression& left,
                            const std::string& operation,
                            const Expression& right,
                            ValueType type) {
    Expression result;
    result.text = "(" + left.text + " " + operation + " " + right.text + ")";
    result.type = type;
    result.hasSideEffect = left.hasSideEffect || right.hasSideEffect;
    return result;
}

/** Returns the source operator implemented by a binary opcode. */
const char* binaryOperator(std::uint32_t opcode) {
    switch (opcode) {
        case 0x0f: case 0x10: case 0x11: return "+";
        case 0x12: case 0x13: return "-";
        case 0x14: case 0x15: return "*";
        case 0x16: case 0x17: return "/";
        case 0x18: return "%";
        case 0x23: return "&";
        case 0x24: return "|";
        case 0x25: return "^";
        case 0x26: case 0x27: case 0x28: return "==";
        case 0x29: case 0x2a: case 0x2b: return "!=";
        case 0x2c: case 0x2d: case 0x2e: return ">";
        case 0x2f: case 0x30: case 0x31: return ">=";
        case 0x32: case 0x33: case 0x34: return "<";
        case 0x35: case 0x36: case 0x37: return "<=";
        case 0x39: return "&&";
        case 0x3a: return "||";
        case 0x3e: return "<<";
        case 0x3f: return ">>";
        default: return NULL;
    }
}

/** Returns true when a binary opcode produces a Boolean integer result. */
bool isComparison(std::uint32_t opcode) {
    return (opcode >= 0x26 && opcode <= 0x3a);
}

/** Joins argument expressions in source order. */
std::string argumentText(const std::vector<Expression>& arguments) {
    std::ostringstream output;
    for (std::size_t index = 0; index < arguments.size(); ++index) {
        if (index != 0) {
            output << ", ";
        }
        output << arguments[index].text;
    }
    return output.str();
}

/** Pops a known number of call arguments while restoring source order. */
std::vector<Expression> popArguments(std::vector<Expression>& stack,
                                     int count, bool& underflow) {
    std::vector<Expression> arguments(static_cast<std::size_t>(std::max(0, count)));
    for (int index = count - 1; index >= 0; --index) {
        arguments[static_cast<std::size_t>(index)]
            = popExpression(stack, underflow);
    }
    return arguments;
}

/** Finds an asynchronous target by its quoted function-name expression. */
int asynchronousTarget(const Module& module, const std::string& quotedName) {
    for (std::size_t index = 0; index < module.functions.size(); ++index) {
        if (quoteSourceString(module.functions[index].name) == quotedName) {
            return static_cast<int>(index);
        }
    }
    return -1;
}

/** Merges an outgoing symbolic stack into a successor's entry stack. */
bool mergeStack(const std::vector<Expression>& outgoing,
                std::vector<Expression>& incoming, bool& hasIncoming) {
    if (!hasIncoming) {
        incoming = outgoing;
        hasIncoming = true;
        return true;
    }
    if (incoming.size() != outgoing.size()) {
        return false;
    }
    for (std::size_t index = 0; index < incoming.size(); ++index) {
        if (!incoming[index].sameAs(outgoing[index])) {
            return false;
        }
    }
    return true;
}

/** Appends an effect expression as a statement and discards pure reads. */
void emitDiscardedExpression(const Expression& expression,
                             std::vector<std::string>& statements) {
    if (expression.hasSideEffect) {
        statements.push_back(expression.text + ";");
    }
}

}  // namespace

LiftedFunction StackLifter::lift(const Module& module,
                                 const FunctionGraph& graph,
                                 const ConfigurationNames& names,
                                 const TypeAnalyzer& types) const {
    LiftedFunction result;
    result.functionIndex = graph.functionIndex;
    if (!graph.isComplete()) {
        result.failureReason = graph.warnings.empty()
            ? "The function has no control-flow graph."
            : graph.warnings.front();
        return result;
    }

    const int functionIndex = graph.functionIndex;
    const int localBase = localSlotBase(graph);
    const int inlineAssemblyLocalSlotAdjustment = 3 - localBase;
    result.returnType = types.functionReturnType(functionIndex);
    result.returnsValue = types.functionReturnsValue(functionIndex);
    for (int parameter = 0;
            parameter < module.functions[functionIndex].parameterCount;
            ++parameter) {
        result.parameterTypes.push_back(
            types.parameterType(functionIndex, parameter));
    }
    result.blocks.resize(graph.blocks.size());
    std::vector<std::vector<Expression> > incoming(graph.blocks.size());
    std::vector<unsigned char> hasIncoming(graph.blocks.size(), 0);
    std::vector<bool> processed(graph.blocks.size(), false);
    std::queue<int> pending;
    hasIncoming[0] = true;
    pending.push(0);

    while (!pending.empty()) {
        const int blockIndex = pending.front();
        pending.pop();
        if (processed[blockIndex]) {
            continue;
        }
        processed[blockIndex] = true;
        const BasicBlock& block = graph.blocks[blockIndex];
        LiftedBlock lifted;
        lifted.index = blockIndex;
        lifted.address = block.startAddress;
        std::vector<Expression> stack = incoming[blockIndex];
        bool underflow = false;
        bool unsupported = false;

        for (std::size_t instructionIndex = 0;
                instructionIndex < block.instructions.size(); ++instructionIndex) {
            const Instruction& instruction = *block.instructions[instructionIndex];
            const std::uint32_t opcode = instruction.opcode;
            if (opcode == 0x00 || opcode == 0x03 || opcode == 0x41
                    || opcode == 0x43 || opcode == 0x44) {
                continue;
            }
            if (opcode == 0x06) {
                Expression value;
                value.type = types.instructionType(instruction.address);
                value.text = value.type == ValueType::floatingPoint
                    ? floatingPointText(instruction.operand)
                    : integerText(instruction.operand);
                value.assembly = expressionAssemblyInstruction(
                    module, functionIndex, inlineAssemblyLocalSlotAdjustment,
                    instruction);
                stack.push_back(value);
            } else if (opcode == 0x07) {
                Expression value;
                value.type = ValueType::string;
                if (instruction.operand < 0
                        || static_cast<std::size_t>(instruction.operand)
                            >= module.runtimeStringAddresses.size()) {
                    unsupported = true;
                    break;
                }
                const std::uint32_t address
                    = module.runtimeStringAddresses[instruction.operand];
                value.text = quoteSourceString(module.strings.find(address)->second.bytes);
                value.assembly = expressionAssemblyInstruction(
                    module, functionIndex, inlineAssemblyLocalSlotAdjustment,
                    instruction);
                stack.push_back(value);
            } else if (opcode == 0x08) {
                Expression value;
                value.text = stackSlotName(module, functionIndex,
                                           instruction.operand, localBase);
                if (instruction.operand <= -2) {
                    const int parameter = module.functions[functionIndex].parameterCount
                        + instruction.operand + 1;
                    value.type = types.parameterType(functionIndex, parameter);
                } else {
                    value.type = types.localType(functionIndex, instruction.operand);
                    result.localTypes[instruction.operand] = value.type;
                }
                value.assembly = expressionAssemblyInstruction(
                    module, functionIndex, inlineAssemblyLocalSlotAdjustment,
                    instruction);
                stack.push_back(value);
            } else if (opcode == 0x09) {
                Expression value;
                value.text = "g_var" + std::to_string(instruction.operand);
                value.type = types.globalType(instruction.operand);
                value.assembly = expressionAssemblyInstruction(
                    module, functionIndex, inlineAssemblyLocalSlotAdjustment,
                    instruction);
                stack.push_back(value);
            } else if (opcode == 0x0a) {
                Expression value;
                value.text = internalValueRead(names, instruction.operand);
                value.type = ValueType::integer;
                value.assembly = expressionAssemblyInstruction(
                    module, functionIndex, inlineAssemblyLocalSlotAdjustment,
                    instruction);
                stack.push_back(value);
            } else if (opcode == 0x04) {
                const int callee = findFunction(
                    module, static_cast<std::uint32_t>(instruction.operand));
                if (callee < 0) {
                    unsupported = true;
                    break;
                }
                std::vector<Expression> arguments = popArguments(
                    stack, module.functions[callee].parameterCount, underflow);
                Expression call;
                call.text = sourceIdentifier(module.functions[callee].name, callee)
                    + "(" + argumentText(arguments) + ")";
                call.type = types.functionReturnType(callee);
                call.hasSideEffect = true;
                call.assembly = expressionAssembly(
                    module, functionIndex, inlineAssemblyLocalSlotAdjustment,
                    arguments, instruction);
                if (types.functionReturnsValue(callee)) {
                    stack.push_back(call);
                } else {
                    lifted.statements.push_back(call.text + ";");
                }
            } else if (opcode == 0x05) {
                const std::uint32_t packed
                    = static_cast<std::uint32_t>(instruction.secondOperand);
                const int count = static_cast<int>(packed & 0xffffU);
                std::vector<Expression> arguments
                    = popArguments(stack, count, underflow);
                const std::uint32_t number
                    = static_cast<std::uint32_t>(instruction.operand);
                const int returnCode = static_cast<int>(packed >> 16);
                if (names.isExtendedAsyncCall(number, count, returnCode)
                        && !arguments.empty()) {
                    const int target = asynchronousTarget(module, arguments[0].text);
                    const int actualCount = target < 0
                        ? static_cast<int>(arguments.size()) - 1
                        : module.functions[target].parameterCount;
                    if (actualCount + 1 > static_cast<int>(arguments.size())) {
                        unsupported = true;
                        break;
                    }
                    std::vector<Expression> actual(arguments.begin() + 1,
                        arguments.begin() + 1 + actualCount);
                    const std::string externalName = target < 0
                        ? unquotedObjectString(module, arguments[0].text)
                        : std::string();
                    if (target < 0 && externalName.empty()) {
                        unsupported = true;
                        break;
                    }
                    if (target < 0) {
                        rememberExternalFunction(result, externalName, actual);
                    }
                    lifted.statements.push_back("asynccall "
                        + (target < 0
                            ? sourceIdentifier(externalName, functionIndex)
                            : sourceIdentifier(module.functions[target].name, target))
                        + "(" + argumentText(actual) + ");");
                } else {
                    const SystemCall* found
                        = names.findSystemCall(number, count, returnCode);
                    if (found == NULL) {
                        std::string inlineAssembly;
                        if (returnCode == 0
                                && inlineAssemblySystemCall(instruction,
                                    arguments, names, inlineAssembly)) {
                            lifted.statements.push_back(inlineAssembly);
                            ++result.inlineAssemblyCount;
                            continue;
                        }
                        unsupported = true;
                        break;
                    }
                    std::ostringstream callText;
                    callText << found->name
                        << "(" << argumentText(arguments) << ")";
                    Expression call;
                    call.text = callText.str();
                    call.type = types.instructionType(instruction.address);
                    call.hasSideEffect = true;
                    call.assembly = expressionAssembly(
                        module, functionIndex, inlineAssemblyLocalSlotAdjustment,
                        arguments, instruction);
                    if ((packed >> 16) != 0) {
                        stack.push_back(call);
                    } else {
                        lifted.statements.push_back(call.text + ";");
                    }
                }
            } else if (opcode == 0x0b) {
                const Expression value = popExpression(stack, underflow);
                lifted.statements.push_back(stackSlotName(
                    module, functionIndex, instruction.operand, localBase)
                    + " = " + value.text + ";");
                if (instruction.operand > 0) {
                    result.localTypes[instruction.operand]
                        = types.localType(functionIndex, instruction.operand);
                }
            } else if (opcode == 0x0c) {
                const Expression value = popExpression(stack, underflow);
                lifted.statements.push_back("g_var"
                    + std::to_string(instruction.operand) + " = "
                    + value.text + ";");
            } else if (opcode == 0x0d) {
                const Expression value = popExpression(stack, underflow);
                std::unordered_map<std::int32_t, std::string>::const_iterator found
                    = names.internalValues.find(instruction.operand);
                lifted.statements.push_back(found == names.internalValues.end()
                    ? "SetIntv(" + std::to_string(instruction.operand) + ", "
                        + value.text + ");"
                    : found->second + " = " + value.text + ";");
            } else if (opcode == 0x0e) {
                emitDiscardedExpression(popExpression(stack, underflow),
                                        lifted.statements);
            } else if (binaryOperator(opcode) != NULL) {
                const Expression right = popExpression(stack, underflow);
                const Expression left = popExpression(stack, underflow);
                Expression value = binaryExpression(left, binaryOperator(opcode), right,
                    isComparison(opcode) ? ValueType::integer
                                         : types.instructionType(instruction.address));
                value.assembly = left.assembly + right.assembly
                    + expressionAssemblyInstruction(module, functionIndex,
                        inlineAssemblyLocalSlotAdjustment, instruction);
                stack.push_back(value);
            } else if (opcode == 0x19) {
                const Expression right = popExpression(stack, underflow);
                const Expression left = popExpression(stack, underflow);
                Expression zero;
                zero.text = "0";
                zero.type = ValueType::integer;
                zero.assembly = left.assembly + right.assembly
                    + expressionAssemblyInstruction(module, functionIndex,
                        inlineAssemblyLocalSlotAdjustment, instruction);
                stack.push_back(zero);
            } else if (opcode == 0x1a || opcode == 0x1b
                    || opcode == 0x1c || opcode == 0x38) {
                const Expression operand = popExpression(stack, underflow);
                const std::string operation = opcode == 0x1a || opcode == 0x1b
                    ? "-" : opcode == 0x1c ? "~" : "!";
                Expression value = unaryExpression(operation, operand,
                    opcode == 0x1b ? ValueType::floatingPoint
                                   : ValueType::integer);
                value.assembly = operand.assembly
                    + expressionAssemblyInstruction(module, functionIndex,
                        inlineAssemblyLocalSlotAdjustment, instruction);
                stack.push_back(value);
            } else if (opcode >= 0x1d && opcode <= 0x22) {
                const bool increment = opcode <= 0x1f;
                std::string target;
                if (opcode == 0x1d || opcode == 0x20) {
                    target = stackSlotName(module, functionIndex,
                                           instruction.operand, localBase);
                    result.localTypes[instruction.operand]
                        = types.localType(functionIndex, instruction.operand);
                } else if (opcode == 0x1e || opcode == 0x21) {
                    target = "g_var" + std::to_string(instruction.operand);
                } else {
                    target = internalValueRead(names, instruction.operand);
                    if (target.compare(0, 8, "GetIntv(") == 0) {
                        target = "SetIntv(" + std::to_string(instruction.operand)
                            + ", GetIntv(" + std::to_string(instruction.operand)
                            + ") " + (increment ? "+ 1);" : "- 1);");
                        lifted.statements.push_back(target);
                        continue;
                    }
                }
                lifted.statements.push_back(target + (increment ? "++;" : "--;"));
            } else if (opcode == 0x3b) {
                lifted.exitKind = ExitKind::jump;
                lifted.target = block.successors.empty() ? -1 : block.successors[0];
            } else if (opcode == 0x3c || opcode == 0x3d) {
                const Expression condition = popExpression(stack, underflow);
                lifted.exitKind = ExitKind::branch;
                lifted.condition = opcode == 0x3c
                    ? condition.text : "!(" + condition.text + ")";
                lifted.target = block.successors.empty() ? -1 : block.successors[0];
                lifted.fallthrough = block.successors.size() < 2
                    ? -1 : block.successors[1];
            } else if (opcode == 0x40) {
                if (stack.empty()) {
                    underflow = true;
                } else {
                    lifted.exitKind = ExitKind::switchCase;
                    lifted.condition = stack.back().text;
                    lifted.switchValue = instruction.operand;
                    lifted.target = block.successors.empty() ? -1 : block.successors[0];
                    lifted.fallthrough = block.successors.size() < 2
                        ? -1 : block.successors[1];
                }
            } else if (opcode == 0x01) {
                lifted.exitKind = ExitKind::returnWithValue;
                lifted.returnValue = popExpression(stack, underflow).text;
            } else if (opcode == 0x02) {
                lifted.exitKind = ExitKind::returnWithoutValue;
            } else if (opcode == 0x42 || opcode == 0x45) {
                const Expression argument = popExpression(stack, underflow);
                lifted.statements.push_back(std::string(opcode == 0x42 ? "Delay(" : "Wait(")
                    + argument.text + ");");
            } else if (opcode == 0x46) {
                const Expression argument = popExpression(stack, underflow);
                Expression call;
                call.text = "IsRunning(" + argument.text + ")";
                call.type = ValueType::integer;
                call.hasSideEffect = true;
                std::vector<Expression> arguments(1, argument);
                call.assembly = expressionAssembly(
                    module, functionIndex, inlineAssemblyLocalSlotAdjustment,
                    arguments, instruction);
                stack.push_back(call);
            } else if (opcode == 0x47) {
                std::vector<Expression> values = popArguments(stack, 5, underflow);
                if (underflow) {
                    break;
                }
                const int target = asynchronousTarget(module, values[0].text);
                if (target >= 0 && module.functions[target].parameterCount > 4) {
                    std::string inlineAssembly;
                    if (!inlineAssemblyCallbs(module, functionIndex,
                                              inlineAssemblyLocalSlotAdjustment,
                                              instruction, values, target,
                                              inlineAssembly)) {
                        unsupported = true;
                        break;
                    }
                    lifted.statements.push_back(inlineAssembly);
                    ++result.inlineAssemblyCount;
                    continue;
                }
                std::string externalName;
                if (target < 0) {
                    externalName = unquotedObjectString(module, values[0].text);
                    if (externalName.empty()) {
                        unsupported = true;
                        break;
                    }
                    while (values.size() > 1 && values.back().text == "0"
                            && !values.back().hasSideEffect) {
                        values.pop_back();
                    }
                } else {
                    values.resize(static_cast<std::size_t>(
                        module.functions[target].parameterCount + 1));
                }
                std::vector<Expression> actual(values.begin() + 1, values.end());
                if (target < 0) {
                    rememberExternalFunction(result, externalName, actual);
                }
                lifted.statements.push_back("asynccall "
                    + (target < 0
                        ? sourceIdentifier(externalName, functionIndex)
                        : sourceIdentifier(module.functions[target].name, target))
                    + "(" + argumentText(actual) + ");");
            } else if (opcode >= 0x48 && opcode <= 0x4c) {
                const Expression operand = popExpression(stack, underflow);
                Expression converted;
                converted.hasSideEffect = operand.hasSideEffect;
                if (opcode == 0x48) {
                    converted.text = "((float)" + operand.text + ")";
                    converted.type = ValueType::floatingPoint;
                } else if (opcode == 0x49) {
                    converted.text = "((string)" + operand.text + ")";
                    converted.type = ValueType::string;
                } else if (opcode == 0x4a) {
                    converted.text = "((int)" + operand.text + ")";
                    converted.type = ValueType::integer;
                } else if (opcode == 0x4b) {
                    converted.text = "((string)" + operand.text + ")";
                    converted.type = ValueType::string;
                } else {
                    converted.text = "0";
                    converted.type = ValueType::integer;
                }
                converted.assembly = operand.assembly
                    + expressionAssemblyInstruction(module, functionIndex,
                        inlineAssemblyLocalSlotAdjustment, instruction);
                stack.push_back(converted);
            } else if (opcode >= 0x4e && opcode <= 0x50) {
                const Expression index = popExpression(stack, underflow);
                Expression value;
                value.type = types.instructionType(instruction.address);
                value.hasSideEffect = index.hasSideEffect;
                if (opcode == 0x4e) {
                    value.text = stackSlotName(module, functionIndex,
                        instruction.operand, localBase) + "[" + index.text + "]";
                    result.localArrayBases.insert(instruction.operand);
                    result.localTypes[instruction.operand] = value.type;
                } else if (opcode == 0x4f) {
                    value.text = instruction.operand == 0
                        ? "GetGlobal(" + index.text + ")"
                        : "g_var" + std::to_string(instruction.operand)
                            + "[" + index.text + "]";
                } else {
                    value.text = "GetIntv("
                        + indexedInternalValue(instruction.operand, index) + ")";
                    value.type = ValueType::integer;
                }
                value.assembly = index.assembly
                    + expressionAssemblyInstruction(module, functionIndex,
                        inlineAssemblyLocalSlotAdjustment, instruction);
                stack.push_back(value);
            } else if (opcode >= 0x51 && opcode <= 0x53) {
                const Expression index = popExpression(stack, underflow);
                const Expression value = popExpression(stack, underflow);
                if (opcode == 0x51) {
                    const std::string array = stackSlotName(module, functionIndex,
                        instruction.operand, localBase);
                    lifted.statements.push_back(array + "[" + index.text
                        + "] = " + value.text + ";");
                    result.localArrayBases.insert(instruction.operand);
                    result.localTypes[instruction.operand]
                        = types.localType(functionIndex, instruction.operand);
                } else if (opcode == 0x52) {
                    lifted.statements.push_back(instruction.operand == 0
                        ? "SetGlobal(" + index.text + ", " + value.text + ");"
                        : "g_var" + std::to_string(instruction.operand)
                            + "[" + index.text + "] = " + value.text + ";");
                } else {
                    lifted.statements.push_back("SetIntv("
                        + indexedInternalValue(instruction.operand, index)
                        + ", " + value.text + ");");
                }
            } else if (opcode >= 0x54 && opcode <= 0x59) {
                const Expression index = popExpression(stack, underflow);
                const bool increment = opcode <= 0x56;
                if (opcode == 0x54 || opcode == 0x57) {
                    const std::string array = stackSlotName(module, functionIndex,
                        instruction.operand, localBase);
                    lifted.statements.push_back(array + "[" + index.text + "]"
                        + (increment ? "++;" : "--;"));
                    result.localArrayBases.insert(instruction.operand);
                    result.localTypes[instruction.operand]
                        = types.localType(functionIndex, instruction.operand);
                } else if (opcode == 0x55 || opcode == 0x58) {
                    if (instruction.operand == 0) {
                        lifted.statements.push_back("SetGlobal(" + index.text
                            + ", GetGlobal(" + index.text + ") "
                            + (increment ? "+ 1);" : "- 1);"));
                    } else {
                        lifted.statements.push_back("g_var"
                            + std::to_string(instruction.operand) + "["
                            + index.text + "]"
                            + (increment ? "++;" : "--;"));
                    }
                } else {
                    const std::string computed
                        = indexedInternalValue(instruction.operand, index);
                    lifted.statements.push_back("SetIntv(" + computed
                        + ", GetIntv(" + computed + ") "
                        + (increment ? "+ 1);" : "- 1);"));
                }
            } else {
                unsupported = true;
                break;
            }
            if (underflow) {
                break;
            }
        }

        if (underflow || unsupported) {
            result.failureReason = underflow
                ? "The operand stack becomes empty before an instruction can read it."
                : "The function uses an instruction that has no safe source spelling.";
            return result;
        }
        if (lifted.exitKind == ExitKind::fallthrough) {
            lifted.fallthrough = block.successors.empty() ? -1 : block.successors[0];
        }
        result.blocks[blockIndex] = lifted;

        for (std::size_t successorIndex = 0;
                successorIndex < block.successors.size(); ++successorIndex) {
            const int successor = block.successors[successorIndex];
            std::vector<Expression> successorStack = stack;
            if (!block.instructions.empty()
                    && block.instructions.back()->opcode == 0x40
                    && successorIndex == 0 && !successorStack.empty()) {
                successorStack.pop_back();
            }
            bool successorHasIncoming = hasIncoming[successor] != 0;
            if (!mergeStack(successorStack, incoming[successor], successorHasIncoming)) {
                result.failureReason
                    = "Incoming operand stacks disagree at a control-flow join.";
                return result;
            }
            hasIncoming[successor] = successorHasIncoming ? 1 : 0;
            if (!processed[successor]) {
                pending.push(successor);
            }
        }
    }

    for (std::size_t index = 0; index < processed.size(); ++index) {
        if (!processed[index]) {
            result.failureReason = "A reachable basic block could not be lifted.";
            return result;
        }
    }
    result.succeeded = true;
    return result;
}

}  // namespace decompiler
}  // namespace sango
