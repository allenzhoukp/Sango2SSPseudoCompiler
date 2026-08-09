#include "Decompiler.h"

#include <algorithm>
#include <queue>

namespace sango {
namespace decompiler {

namespace {

/** Combines two concrete type requirements without hiding a disagreement. */
ValueType combineTypes(ValueType left, ValueType right) {
    if (left == ValueType::unknown) {
        return right;
    }
    if (right == ValueType::unknown || left == right) {
        return left;
    }
    return ValueType::conflict;
}

/** Pops one symbolic type node and reports an empty stack as no node. */
int popNode(std::vector<int>& stack) {
    if (stack.empty()) {
        return -1;
    }
    const int result = stack.back();
    stack.pop_back();
    return result;
}

/** Returns the function-table index for a direct call address. */
int functionAtAddress(const Module& module, std::uint32_t address) {
    for (std::size_t index = 0; index < module.functions.size(); ++index) {
        if (module.functions[index].entryAddress == address) {
            return static_cast<int>(index);
        }
    }
    return -1;
}

/** Returns true when a function has at least one value-return instruction. */
bool hasValueReturn(const FunctionGraph& graph) {
    for (std::size_t blockIndex = 0; blockIndex < graph.blocks.size(); ++blockIndex) {
        const BasicBlock& block = graph.blocks[blockIndex];
        for (std::size_t index = 0; index < block.instructions.size(); ++index) {
            if (block.instructions[index]->opcode == 0x01) {
                return true;
            }
        }
    }
    return false;
}

}  // namespace

int TypeAnalyzer::makeNode() {
    TypeNode node;
    node.parent = static_cast<int>(nodes_.size());
    nodes_.push_back(node);
    return static_cast<int>(nodes_.size()) - 1;
}

int TypeAnalyzer::findNode(int node) {
    if (node < 0 || node >= static_cast<int>(nodes_.size())) {
        return -1;
    }
    if (nodes_[node].parent != node) {
        nodes_[node].parent = findNode(nodes_[node].parent);
    }
    return nodes_[node].parent;
}

void TypeAnalyzer::requireType(int node, ValueType type) {
    const int root = findNode(node);
    if (root >= 0) {
        nodes_[root].type = combineTypes(nodes_[root].type, type);
    }
}

void TypeAnalyzer::joinNodes(int left, int right) {
    int leftRoot = findNode(left);
    int rightRoot = findNode(right);
    if (leftRoot < 0 || rightRoot < 0 || leftRoot == rightRoot) {
        return;
    }
    nodes_[rightRoot].parent = leftRoot;
    nodes_[leftRoot].type
        = combineTypes(nodes_[leftRoot].type, nodes_[rightRoot].type);
}

int TypeAnalyzer::localNode(int functionIndex, int slot) {
    std::map<int, int>& locals = localNodes_[functionIndex];
    std::map<int, int>::iterator found = locals.find(slot);
    if (found != locals.end()) {
        return found->second;
    }
    const int node = makeNode();
    locals[slot] = node;
    return node;
}

int TypeAnalyzer::globalNode(int slot) {
    std::map<int, int>::iterator found = globalNodes_.find(slot);
    if (found != globalNodes_.end()) {
        return found->second;
    }
    const int node = makeNode();
    globalNodes_[slot] = node;
    return node;
}

int TypeAnalyzer::instructionNode(const Instruction& instruction) {
    std::map<std::uint32_t, int>::iterator found
        = instructionNodes_.find(instruction.address);
    if (found != instructionNodes_.end()) {
        return found->second;
    }
    const int node = makeNode();
    instructionNodes_[instruction.address] = node;
    return node;
}

ValueType TypeAnalyzer::resolvedType(int node) const {
    if (node < 0 || node >= static_cast<int>(nodes_.size())) {
        return ValueType::unknown;
    }
    int root = node;
    while (nodes_[root].parent != root) {
        root = nodes_[root].parent;
    }
    return nodes_[root].type;
}

void TypeAnalyzer::analyze(const Module& module,
                           const std::vector<FunctionGraph>& graphs,
                           const ConfigurationNames&) {
    nodes_.clear();
    returnNodes_.clear();
    returnsValue_.clear();
    parameterNodes_.clear();
    localNodes_.clear();
    globalNodes_.clear();
    instructionNodes_.clear();

    returnNodes_.resize(module.functions.size());
    parameterNodes_.resize(module.functions.size());
    localNodes_.resize(module.functions.size());
    std::vector<bool> valueReturns(module.functions.size(), false);
    for (std::size_t functionIndex = 0;
            functionIndex < module.functions.size(); ++functionIndex) {
        returnNodes_[functionIndex] = makeNode();
        parameterNodes_[functionIndex].resize(
            module.functions[functionIndex].parameterCount);
        for (int parameter = 0;
                parameter < module.functions[functionIndex].parameterCount;
                ++parameter) {
            parameterNodes_[functionIndex][parameter] = makeNode();
        }
        if (functionIndex < graphs.size()) {
            valueReturns[functionIndex] = hasValueReturn(graphs[functionIndex]);
        }
    }
    returnsValue_ = valueReturns;

    for (std::size_t functionIndex = 0;
            functionIndex < graphs.size(); ++functionIndex) {
        const FunctionGraph& graph = graphs[functionIndex];
        if (graph.blocks.empty()) {
            continue;
        }
        std::vector<std::vector<int> > incoming(graph.blocks.size());
        std::vector<bool> hasIncoming(graph.blocks.size(), false);
        std::queue<int> pending;
        hasIncoming[0] = true;
        pending.push(0);
        int visits = 0;

        while (!pending.empty() && visits < static_cast<int>(graph.blocks.size()) * 8 + 8) {
            const int blockIndex = pending.front();
            pending.pop();
            ++visits;
            std::vector<int> stack = incoming[blockIndex];
            const BasicBlock& block = graph.blocks[blockIndex];

            for (std::size_t instructionIndex = 0;
                    instructionIndex < block.instructions.size();
                    ++instructionIndex) {
                const Instruction& instruction
                    = *block.instructions[instructionIndex];
                const std::uint32_t opcode = instruction.opcode;
                if (opcode == 0x06 || opcode == 0x07) {
                    const int node = instructionNode(instruction);
                    if (opcode == 0x07) {
                        requireType(node, ValueType::string);
                    }
                    // PUSH constants are left untyped so that a later float
                    // use (e.g. CMPLF) can type the raw bits as a float.
                    stack.push_back(node);
                } else if (opcode == 0x08) {
                    if (instruction.operand <= -2) {
                        const int parameter = module.functions[functionIndex].parameterCount
                            + instruction.operand + 1;
                        stack.push_back(parameter >= 0
                                && parameter < static_cast<int>(
                                    parameterNodes_[functionIndex].size())
                            ? parameterNodes_[functionIndex][parameter]
                            : instructionNode(instruction));
                    } else {
                        stack.push_back(localNode(static_cast<int>(functionIndex),
                                                  instruction.operand));
                    }
                } else if (opcode == 0x09) {
                    stack.push_back(globalNode(instruction.operand));
                } else if (opcode == 0x0a) {
                    const int node = instructionNode(instruction);
                    requireType(node, ValueType::integer);
                    stack.push_back(node);
                } else if (opcode == 0x04) {
                    const int callee = functionAtAddress(
                        module, static_cast<std::uint32_t>(instruction.operand));
                    if (callee >= 0) {
                        const int count = module.functions[callee].parameterCount;
                        std::vector<int> arguments(count, -1);
                        for (int argument = count - 1; argument >= 0; --argument) {
                            arguments[argument] = popNode(stack);
                        }
                        for (int argument = 0; argument < count; ++argument) {
                            joinNodes(arguments[argument],
                                      parameterNodes_[callee][argument]);
                        }
                        if (valueReturns[callee]) {
                            stack.push_back(returnNodes_[callee]);
                        }
                    }
                } else if (opcode == 0x05) {
                    const std::uint32_t packed
                        = static_cast<std::uint32_t>(instruction.secondOperand);
                    const int count = static_cast<int>(packed & 0xffffU);
                    for (int argument = 0; argument < count; ++argument) {
                        popNode(stack);
                    }
                    const int returnCode = static_cast<int>(packed >> 16);
                    if (returnCode != 0) {
                        const int node = instructionNode(instruction);
                        requireType(node, returnCode == 2
                            ? ValueType::floatingPoint
                            : returnCode == 3 ? ValueType::string
                            : ValueType::integer);
                        stack.push_back(node);
                    }
                } else if (opcode == 0x0b) {
                    joinNodes(popNode(stack), localNode(
                        static_cast<int>(functionIndex), instruction.operand));
                } else if (opcode == 0x0c) {
                    joinNodes(popNode(stack), globalNode(instruction.operand));
                } else if (opcode == 0x0d) {
                    requireType(popNode(stack), ValueType::integer);
                } else if (opcode == 0x0e) {
                    popNode(stack);
                } else if ((opcode >= 0x0f && opcode <= 0x18)
                        || (opcode >= 0x23 && opcode <= 0x28)
                        || (opcode >= 0x29 && opcode <= 0x37)
                        || opcode == 0x39 || opcode == 0x3a
                        || opcode == 0x3e || opcode == 0x3f) {
                    const int right = popNode(stack);
                    const int left = popNode(stack);
                    const int result = instructionNode(instruction);
                    const bool stringOperation = opcode == 0x11 || opcode == 0x28
                        || opcode == 0x2b || opcode == 0x2e || opcode == 0x31
                        || opcode == 0x34 || opcode == 0x37;
                    const bool floatOperation = opcode == 0x10 || opcode == 0x13
                        || opcode == 0x15 || opcode == 0x17 || opcode == 0x27
                        || opcode == 0x2a || opcode == 0x2d || opcode == 0x30
                        || opcode == 0x33 || opcode == 0x36;
                    const bool comparison = opcode >= 0x26 && opcode <= 0x3a;
                    const ValueType operandType = stringOperation ? ValueType::string
                        : floatOperation ? ValueType::floatingPoint
                        : ValueType::integer;
                    requireType(left, operandType);
                    requireType(right, operandType);
                    requireType(result, comparison ? ValueType::integer : operandType);
                    stack.push_back(result);
                } else if (opcode == 0x19) {
                    popNode(stack);
                    popNode(stack);
                    const int node = instructionNode(instruction);
                    requireType(node, ValueType::integer);
                    stack.push_back(node);
                } else if (opcode == 0x1a || opcode == 0x1b || opcode == 0x1c
                        || opcode == 0x38) {
                    const int operand = popNode(stack);
                    const int result = instructionNode(instruction);
                    requireType(operand, opcode == 0x1b
                        ? ValueType::floatingPoint : ValueType::integer);
                    requireType(result, opcode == 0x1b
                        ? ValueType::floatingPoint : ValueType::integer);
                    stack.push_back(result);
                } else if (opcode == 0x3c || opcode == 0x3d) {
                    requireType(popNode(stack), ValueType::integer);
                } else if (opcode == 0x42 || opcode == 0x45) {
                    popNode(stack);
                } else if (opcode == 0x46) {
                    requireType(popNode(stack), ValueType::string);
                    const int result = instructionNode(instruction);
                    requireType(result, ValueType::integer);
                    stack.push_back(result);
                } else if (opcode == 0x47) {
                    for (int value = 0; value < 5; ++value) {
                        popNode(stack);
                    }
                } else if (opcode == 0x48 || opcode == 0x49 || opcode == 0x4a
                        || opcode == 0x4b || opcode == 0x4c) {
                    const int operand = popNode(stack);
                    const int result = instructionNode(instruction);
                    if (opcode == 0x48) {
                        requireType(operand, ValueType::integer);
                        requireType(result, ValueType::floatingPoint);
                    } else if (opcode == 0x49) {
                        requireType(operand, ValueType::integer);
                        requireType(result, ValueType::string);
                    } else if (opcode == 0x4a) {
                        requireType(operand, ValueType::floatingPoint);
                        requireType(result, ValueType::integer);
                    } else if (opcode == 0x4b) {
                        requireType(operand, ValueType::floatingPoint);
                        requireType(result, ValueType::string);
                    } else {
                        requireType(result, ValueType::integer);
                    }
                    stack.push_back(result);
                } else if (opcode >= 0x4e && opcode <= 0x50) {
                    requireType(popNode(stack), ValueType::integer);
                    const int result = instructionNode(instruction);
                    if (opcode == 0x4e) {
                        joinNodes(result, localNode(
                            static_cast<int>(functionIndex), instruction.operand));
                    } else if (opcode == 0x4f) {
                        joinNodes(result, globalNode(instruction.operand));
                    } else {
                        requireType(result, ValueType::integer);
                    }
                    stack.push_back(result);
                } else if (opcode >= 0x51 && opcode <= 0x53) {
                    requireType(popNode(stack), ValueType::integer);
                    const int value = popNode(stack);
                    if (opcode == 0x51) {
                        joinNodes(value, localNode(
                            static_cast<int>(functionIndex), instruction.operand));
                    } else if (opcode == 0x52) {
                        joinNodes(value, globalNode(instruction.operand));
                    } else {
                        requireType(value, ValueType::integer);
                    }
                } else if (opcode >= 0x54 && opcode <= 0x59) {
                    requireType(popNode(stack), ValueType::integer);
                    if (opcode == 0x54 || opcode == 0x57) {
                        requireType(localNode(static_cast<int>(functionIndex),
                                              instruction.operand),
                                    ValueType::integer);
                    } else if (opcode == 0x55 || opcode == 0x58) {
                        requireType(globalNode(instruction.operand),
                                    ValueType::integer);
                    }
                } else if (opcode == 0x5b) {
                    const int result = instructionNode(instruction);
                    requireType(result, ValueType::integer);
                    stack.push_back(result);
                } else if (opcode == 0x01) {
                    joinNodes(popNode(stack), returnNodes_[functionIndex]);
                }
            }

            for (std::size_t successorIndex = 0;
                    successorIndex < block.successors.size(); ++successorIndex) {
                const int successor = block.successors[successorIndex];
                std::vector<int> successorStack = stack;
                if (!block.instructions.empty()
                        && block.instructions.back()->opcode == 0x40
                        && successorIndex == 0 && !successorStack.empty()) {
                    successorStack.pop_back();
                }
                if (!hasIncoming[successor]) {
                    incoming[successor] = successorStack;
                    hasIncoming[successor] = true;
                    pending.push(successor);
                } else if (incoming[successor].size() == successorStack.size()) {
                    for (std::size_t position = 0; position < successorStack.size(); ++position) {
                        joinNodes(incoming[successor][position], successorStack[position]);
                    }
                }
            }
        }
    }
}

ValueType TypeAnalyzer::functionReturnType(int functionIndex) const {
    return functionIndex >= 0 && functionIndex < static_cast<int>(returnNodes_.size())
        ? resolvedType(returnNodes_[functionIndex]) : ValueType::unknown;
}

bool TypeAnalyzer::functionReturnsValue(int functionIndex) const {
    return functionIndex >= 0 && functionIndex < static_cast<int>(returnsValue_.size())
        && returnsValue_[functionIndex];
}

ValueType TypeAnalyzer::parameterType(int functionIndex, int parameterIndex) const {
    if (functionIndex < 0 || functionIndex >= static_cast<int>(parameterNodes_.size())
            || parameterIndex < 0
            || parameterIndex >= static_cast<int>(parameterNodes_[functionIndex].size())) {
        return ValueType::unknown;
    }
    return resolvedType(parameterNodes_[functionIndex][parameterIndex]);
}

ValueType TypeAnalyzer::localType(int functionIndex, int slot) const {
    if (functionIndex < 0 || functionIndex >= static_cast<int>(localNodes_.size())) {
        return ValueType::unknown;
    }
    std::map<int, int>::const_iterator found = localNodes_[functionIndex].find(slot);
    return found == localNodes_[functionIndex].end()
        ? ValueType::unknown : resolvedType(found->second);
}

ValueType TypeAnalyzer::globalType(int slot) const {
    std::map<int, int>::const_iterator found = globalNodes_.find(slot);
    return found == globalNodes_.end()
        ? ValueType::unknown : resolvedType(found->second);
}

ValueType TypeAnalyzer::instructionType(std::uint32_t address) const {
    std::map<std::uint32_t, int>::const_iterator found
        = instructionNodes_.find(address);
    return found == instructionNodes_.end()
        ? ValueType::unknown : resolvedType(found->second);
}

}  // namespace decompiler
}  // namespace sango
