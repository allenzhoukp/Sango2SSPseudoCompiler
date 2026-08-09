#include "Decompiler.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <limits>
#include <set>
#include <sstream>

namespace sango {
namespace decompiler {

namespace {

/** Escapes one string for a JavaScript Object Notation string value. */
std::string quoteReportString(const std::string& value) {
    std::ostringstream output;
    output << '"';
    for (std::size_t index = 0; index < value.size(); ++index) {
        const unsigned char byte = static_cast<unsigned char>(value[index]);
        switch (byte) {
            case '\\': output << "\\\\"; break;
            case '"': output << "\\\""; break;
            case '\n': output << "\\n"; break;
            case '\r': output << "\\r"; break;
            case '\t': output << "\\t"; break;
            default:
                if (byte >= 0x20 && byte <= 0x7e) {
                    output << static_cast<char>(byte);
                } else {
                    output << "\\u00" << std::hex << std::uppercase
                           << std::setw(2) << std::setfill('0')
                           << static_cast<unsigned int>(byte)
                           << std::dec << std::nouppercase << std::setfill(' ');
                }
        }
    }
    output << '"';
    return output.str();
}

/** Returns a stable label for one address inside a naked function. */
std::string assemblyLabel(int functionIndex, std::uint32_t address) {
    std::ostringstream output;
    output << "assembly_function_" << functionIndex << "_address_"
           << std::hex << std::uppercase << address;
    return output.str();
}

/** Finds a function table record whose entry has the requested address. */
int functionAtAddress(const Module& module, std::uint32_t address) {
    for (std::size_t index = 0; index < module.functions.size(); ++index) {
        if (module.functions[index].entryAddress == address) {
            return static_cast<int>(index);
        }
    }
    return -1;
}

/** Returns a source or assembly symbol for one direct control-flow target. */
std::string targetName(const Module& module, int ownerFunction,
                       std::uint32_t address) {
    const int targetFunction = functionAtAddress(module, address);
    if (targetFunction >= 0) {
        return sourceIdentifier(module.functions[targetFunction].name,
                                targetFunction);
    }
    return assemblyLabel(ownerFunction, address);
}

/** Writes one decoded instruction in syntax accepted inside __asm. */
void writeAssemblyInstruction(std::ostream& output, const Module& module,
                              int functionIndex,
                              const Instruction& instruction) {
    const InstructionDescription* description
        = instructionDescription(instruction.opcode);
    output << "        " << description->name;
    if (instruction.operandCount >= 1) {
        output << ' ';
        if (instruction.opcode == 0x04 || instruction.opcode == 0x3b
                || instruction.opcode == 0x3c || instruction.opcode == 0x3d) {
            output << targetName(module, functionIndex,
                static_cast<std::uint32_t>(instruction.operand));
        } else if (instruction.opcode == 0x07 && instruction.operand >= 0
                && static_cast<std::size_t>(instruction.operand)
                    < module.runtimeStringAddresses.size()) {
            const std::uint32_t address
                = module.runtimeStringAddresses[instruction.operand];
            output << quoteSourceString(module.strings.find(address)->second.bytes);
        } else if (instruction.opcode == 0x05) {
            output << "0x" << std::hex << std::uppercase
                   << static_cast<std::uint32_t>(instruction.operand)
                   << std::dec << std::nouppercase;
        } else {
            output << instruction.operand;
        }
    }
    if (instruction.operandCount == 2) {
        output << ", ";
        if (instruction.opcode == 0x40) {
            output << targetName(module, functionIndex,
                static_cast<std::uint32_t>(instruction.secondOperand));
        } else if (instruction.opcode == 0x05) {
            const std::uint32_t packed
                = static_cast<std::uint32_t>(instruction.secondOperand);
            output << '(' << (packed & 0xffffU) << " | ("
                   << (packed >> 16) << " << 16))";
        } else {
            output << instruction.secondOperand;
        }
    }
    output << '\n';
}

/** Writes a complete function as readable naked virtual-machine assembly. */
void writeAssemblyFunction(std::ostream& output, const Module& module,
                           const FunctionGraph& graph,
                           const TypeAnalyzer& types) {
    const int functionIndex = graph.functionIndex;
    const FunctionRecord& record = module.functions[functionIndex];
    const ValueType returnType = types.functionReturnsValue(functionIndex)
        ? types.functionReturnType(functionIndex) : ValueType::unknown;
    output << (types.functionReturnsValue(functionIndex)
        ? sourceTypeName(returnType) : "void")
        << " naked " << sourceIdentifier(record.name, functionIndex)
        << '(';
    for (int parameter = 0; parameter < record.parameterCount; ++parameter) {
        if (parameter != 0) {
            output << ", ";
        }
        output << sourceTypeName(types.parameterType(functionIndex, parameter))
               << " a" << parameter;
    }
    output << ')';
    if (record.callSign != 0) {
        output << " callsign " << record.callSign;
    }
    output << " __asm {\n";

    std::set<std::uint32_t> targets;
    for (std::size_t blockIndex = 0; blockIndex < graph.blocks.size(); ++blockIndex) {
        const BasicBlock& block = graph.blocks[blockIndex];
        if (!block.instructions.empty()) {
            const Instruction& last = *block.instructions.back();
            if (last.opcode == 0x3b || last.opcode == 0x3c
                    || last.opcode == 0x3d) {
                targets.insert(static_cast<std::uint32_t>(last.operand));
            } else if (last.opcode == 0x40) {
                targets.insert(static_cast<std::uint32_t>(last.secondOperand));
            }
        }
    }
    std::vector<std::uint32_t> addresses(graph.reachableAddresses.begin(),
                                         graph.reachableAddresses.end());
    for (std::size_t index = 0; index < addresses.size(); ++index) {
        const std::uint32_t address = addresses[index];
        if (targets.count(address) != 0
                && address != record.entryAddress
                && functionAtAddress(module, address) < 0) {
            output << assemblyLabel(functionIndex, address) << ":\n";
        }
        writeAssemblyInstruction(output, module, functionIndex,
                                 module.instructions.find(address)->second);
    }
    output << "}\n\n";
}

/** Returns the local-slot count encoded by a normal function's STACK instruction. */
int localCount(const FunctionGraph& graph) {
    int stackSize = 0;
    int smallestLocal = std::numeric_limits<int>::max();
    for (std::size_t blockIndex = 0; blockIndex < graph.blocks.size(); ++blockIndex) {
        const BasicBlock& block = graph.blocks[blockIndex];
        for (std::size_t index = 0; index < block.instructions.size(); ++index) {
            if (block.instructions[index]->opcode == 0x03) {
                stackSize = block.instructions[index]->operand;
            }
            const Instruction& instruction = *block.instructions[index];
            if ((instruction.opcode == 0x08 || instruction.opcode == 0x0b
                    || instruction.opcode == 0x1d || instruction.opcode == 0x20
                    || instruction.opcode == 0x4e || instruction.opcode == 0x51
                    || instruction.opcode == 0x54 || instruction.opcode == 0x57)
                    && instruction.operand > 0) {
                smallestLocal = std::min(smallestLocal, instruction.operand);
            }
        }
    }
    return std::max(0, smallestLocal <= 2 ? stackSize : stackSize - 2);
}

/** Returns the first local slot used by the object's frame convention. */
int localBase(const FunctionGraph& graph) {
    int smallestLocal = std::numeric_limits<int>::max();
    for (std::size_t blockIndex = 0; blockIndex < graph.blocks.size(); ++blockIndex) {
        const BasicBlock& block = graph.blocks[blockIndex];
        for (std::size_t index = 0; index < block.instructions.size(); ++index) {
            const Instruction& instruction = *block.instructions[index];
            if ((instruction.opcode == 0x08 || instruction.opcode == 0x0b
                    || instruction.opcode == 0x1d || instruction.opcode == 0x20
                    || instruction.opcode == 0x4e || instruction.opcode == 0x51
                    || instruction.opcode == 0x54 || instruction.opcode == 0x57)
                    && instruction.operand > 0) {
                smallestLocal = std::min(smallestLocal, instruction.operand);
            }
        }
    }
    return smallestLocal <= 2 ? 1 : 3;
}

/** Returns true when an instruction directly reads or writes one local slot. */
bool isDirectLocalAccess(const Instruction& instruction) {
    return instruction.opcode == 0x08 || instruction.opcode == 0x0b
        || instruction.opcode == 0x1d || instruction.opcode == 0x20;
}

/** Infers a local array length from the next independently addressed slot. */
int localArrayLength(const FunctionGraph& graph,
                     const LiftedFunction& lifted, int base) {
    const int firstSlot = localBase(graph);
    int boundary = firstSlot + localCount(graph);
    for (std::size_t blockIndex = 0; blockIndex < graph.blocks.size(); ++blockIndex) {
        const BasicBlock& block = graph.blocks[blockIndex];
        for (std::size_t index = 0; index < block.instructions.size(); ++index) {
            const Instruction& instruction = *block.instructions[index];
            if (isDirectLocalAccess(instruction) && instruction.operand > base) {
                boundary = std::min(boundary, instruction.operand);
            }
        }
    }
    std::set<int>::const_iterator next = lifted.localArrayBases.upper_bound(base);
    if (next != lifted.localArrayBases.end()) {
        boundary = std::min(boundary, *next);
    }
    return std::max(1, boundary - base);
}

/** Returns every global slot used as the base of indexed object access. */
std::set<int> globalArrayBases(const Module& module) {
    std::set<int> result;
    for (std::map<std::uint32_t, Instruction>::const_iterator
            found = module.instructions.begin();
            found != module.instructions.end(); ++found) {
        const std::uint32_t opcode = found->second.opcode;
        if (opcode == 0x4f || opcode == 0x52
                || opcode == 0x55 || opcode == 0x58) {
            if (found->second.operand <= 0) {
                continue;
            }
            result.insert(found->second.operand);
        }
    }
    return result;
}

/** Infers a global array length from the next independently addressed slot. */
int globalArrayLength(const Module& module,
                      const std::set<int>& arrayBases, int base) {
    int boundary = static_cast<int>(module.globalCount) + 1;
    for (std::map<std::uint32_t, Instruction>::const_iterator
            found = module.instructions.begin();
            found != module.instructions.end(); ++found) {
        const Instruction& instruction = found->second;
        const bool direct = instruction.opcode == 0x09
            || instruction.opcode == 0x0c || instruction.opcode == 0x1e
            || instruction.opcode == 0x21;
        if (direct && instruction.operand > base) {
            boundary = std::min(boundary, instruction.operand);
        }
    }
    std::set<int>::const_iterator next = arrayBases.upper_bound(base);
    if (next != arrayBases.end()) {
        boundary = std::min(boundary, *next);
    }
    return std::max(1, boundary - base);
}

/** Returns the same-object functions that must precede one high-level caller. */
std::set<int> functionDependencies(const Module& module,
                                   const FunctionGraph& graph,
                                   const LiftedFunction& lifted) {
    std::set<int> dependencies;
    const int functionIndex = graph.functionIndex;
    for (std::set<std::uint32_t>::const_iterator address
            = graph.reachableAddresses.begin();
            address != graph.reachableAddresses.end(); ++address) {
        const Instruction& instruction = module.instructions.find(*address)->second;
        if (instruction.opcode == 0x04) {
            const int target = functionAtAddress(
                module, static_cast<std::uint32_t>(instruction.operand));
            if (target >= 0 && target != functionIndex) {
                dependencies.insert(target);
            }
        }
    }
    for (std::size_t block = 0; block < lifted.blocks.size(); ++block) {
        for (std::size_t line = 0;
                line < lifted.blocks[block].statements.size(); ++line) {
            const std::string& text = lifted.blocks[block].statements[line];
            for (std::size_t target = 0; target < module.functions.size(); ++target) {
                if (static_cast<int>(target) != functionIndex
                        && text.find("asynccall " + sourceIdentifier(
                            module.functions[target].name,
                            static_cast<int>(target)) + "(") != std::string::npos) {
                    dependencies.insert(static_cast<int>(target));
                }
            }
        }
    }
    return dependencies;
}

/** Produces a stable dependency order and reports functions left in cycles. */
std::vector<int> functionEmissionOrder(
        const Module& module,
        const std::vector<FunctionGraph>& graphs,
        const std::vector<LiftedFunction>& functions,
        std::set<int>& cyclicFunctions) {
    const int count = static_cast<int>(module.functions.size());
    std::vector<int> dependencyCount(static_cast<std::size_t>(count), 0);
    std::vector<std::vector<int> > dependents(static_cast<std::size_t>(count));
    for (int caller = 0; caller < count; ++caller) {
        if (!functions[caller].succeeded) {
            continue;
        }
        const std::set<int> dependencies
            = functionDependencies(module, graphs[caller], functions[caller]);
        dependencyCount[caller] = static_cast<int>(dependencies.size());
        for (std::set<int>::const_iterator dependency = dependencies.begin();
                dependency != dependencies.end(); ++dependency) {
            dependents[*dependency].push_back(caller);
        }
    }

    std::set<int> ready;
    for (int index = 0; index < count; ++index) {
        if (dependencyCount[index] == 0) {
            ready.insert(index);
        }
    }
    std::vector<int> order;
    while (!ready.empty()) {
        const int next = *ready.begin();
        ready.erase(ready.begin());
        order.push_back(next);
        for (std::size_t index = 0; index < dependents[next].size(); ++index) {
            const int dependent = dependents[next][index];
            if (--dependencyCount[dependent] == 0) {
                ready.insert(dependent);
            }
        }
    }
    for (int index = 0; index < count; ++index) {
        if (dependencyCount[index] != 0) {
            cyclicFunctions.insert(index);
            order.push_back(index);
        }
    }
    return order;
}

/** Returns true when a caller still precedes a dependency after reordering. */
bool hasUnresolvedForwardCall(const Module& module,
                              const FunctionGraph& graph,
                              const LiftedFunction& lifted,
                              const std::vector<int>& position) {
    const std::set<int> dependencies
        = functionDependencies(module, graph, lifted);
    for (std::set<int>::const_iterator dependency = dependencies.begin();
            dependency != dependencies.end(); ++dependency) {
        if (position[*dependency] > position[graph.functionIndex]) {
            return true;
        }
    }
    return false;
}

/** Writes one lifted function signature, declarations, and body. */
void writeHighLevelFunction(std::ostream& output, const Module& module,
                            const FunctionGraph& graph,
                            const LiftedFunction& lifted,
                            const TypeAnalyzer& types,
                            bool structuredMode) {
    const int functionIndex = graph.functionIndex;
    const FunctionRecord& record = module.functions[functionIndex];
    output << (types.functionReturnsValue(functionIndex)
        ? sourceTypeName(lifted.returnType) : "void")
        << ' ' << sourceIdentifier(record.name, functionIndex) << '(';
    for (int parameter = 0; parameter < record.parameterCount; ++parameter) {
        if (parameter != 0) {
            output << ", ";
        }
        output << sourceTypeName(lifted.parameterTypes[parameter])
               << " a" << parameter;
    }
    output << ')';
    if (record.callSign != 0) {
        output << " callsign " << record.callSign;
    }
    output << " {\n";

    const int count = localCount(graph);
    const int firstSlot = localBase(graph);
    const int afterLastSlot = firstSlot + count;
    int slot = firstSlot;
    while (slot < afterLastSlot) {
        const int displayed = slot - firstSlot + 1;
        if (lifted.localArrayBases.count(slot) != 0) {
            const int length = localArrayLength(graph, lifted, slot);
            output << "    " << sourceTypeName(types.localType(functionIndex, slot))
                   << " v" << displayed << '[' << length << "];\n";
            slot += length;
            continue;
        }

        const int firstInRun = slot;
        const ValueType runType = types.localType(functionIndex, slot);
        while (slot + 1 < afterLastSlot
                && lifted.localArrayBases.count(slot + 1) == 0
                && types.localType(functionIndex, slot + 1) == runType) {
            ++slot;
        }
        output << "    " << sourceTypeName(runType) << ' ';
        for (int item = firstInRun; item <= slot; ++item) {
            if (item != firstInRun) {
                output << ", ";
            }
            output << 'v' << (item - firstSlot + 1);
        }
        output << ";\n";
        ++slot;
    }
    if (count != 0) {
        output << '\n';
    }
    if (lifted.usesDispatcher) {
        output << "    int control_state_" << functionIndex << ";\n\n";
    }

    if (structuredMode) {
        for (std::size_t line = 0; line < lifted.structuredBody.size(); ++line) {
            const std::string& text = lifted.structuredBody[line];
            if (!text.empty() && text[text.size() - 1] == ':') {
                output << text << '\n';
            } else {
                output << "    " << text << '\n';
            }
        }
    } else {
        for (std::size_t blockIndex = 0;
                blockIndex < lifted.blocks.size(); ++blockIndex) {
            const LiftedBlock& block = lifted.blocks[blockIndex];
            output << "function_" << functionIndex << "_block_"
                   << blockIndex << ":\n";
            for (std::size_t statement = 0;
                    statement < block.statements.size(); ++statement) {
                output << "    " << block.statements[statement] << '\n';
            }
            if (block.exitKind == ExitKind::jump) {
                output << "    goto function_" << functionIndex << "_block_"
                       << block.target << ";\n";
            } else if (block.exitKind == ExitKind::branch) {
                output << "    if (" << block.condition << ") goto function_"
                       << functionIndex << "_block_" << block.target << ";\n"
                       << "    goto function_" << functionIndex << "_block_"
                       << block.fallthrough << ";\n";
            } else if (block.exitKind == ExitKind::returnWithoutValue) {
                output << "    return;\n";
            } else if (block.exitKind == ExitKind::returnWithValue) {
                output << "    return " << block.returnValue << ";\n";
            } else if (block.exitKind == ExitKind::fallthrough
                    && block.fallthrough >= 0) {
                output << "    goto function_" << functionIndex << "_block_"
                       << block.fallthrough << ";\n";
            } else if (block.exitKind == ExitKind::switchCase) {
                output << "    if (" << block.condition << " == "
                       << block.switchValue << ") goto function_" << functionIndex
                       << "_block_" << block.target << ";\n"
                       << "    goto function_" << functionIndex << "_block_"
                       << block.fallthrough << ";\n";
            }
        }
    }
    output << "}\n\n";
}

}  // namespace

bool DecompilationReport::write(const std::string& path,
                                std::string& error) const {
    std::ofstream output(path.c_str());
    if (!output) {
        error = "Unable to create report " + path + ".";
        return false;
    }
    output << "{\n"
           << "  \"input\": " << quoteReportString(inputPath) << ",\n"
           << "  \"source_output\": " << quoteReportString(outputPath) << ",\n"
           << "  \"mode\": " << quoteReportString(mode) << ",\n"
           << "  \"object\": {\n"
           << "    \"decoded_instructions\": " << decodedInstructionCount << ",\n"
           << "    \"unknown_words\": " << unknownWordCount << "\n"
           << "  },\n"
           << "  \"summary\": {\n"
           << "    \"high_level_functions\": " << highLevelFunctionCount << ",\n"
           << "    \"structured_functions\": " << structuredFunctionCount << ",\n"
           << "    \"naked_assembly_functions\": " << assemblyFunctionCount << ",\n"
           << "    \"inline_assembly_functions\": "
           << inlineAssemblyFunctionCount << ",\n"
           << "    \"inline_assembly_blocks\": "
           << inlineAssemblyBlockCount << ",\n"
           << "    \"automatically_included_functions\": "
           << omittedIncludeFunctionCount << ",\n"
           << "    \"dispatcher_functions\": " << dispatcherFunctionCount << ",\n"
           << "    \"residual_gotos\": " << residualGotoCount << "\n"
           << "  },\n"
           << "  \"unchanged_compiler_preflight\": {\n"
           << "    \"generated_source_bytes\": " << generatedSourceBytes << ",\n"
           << "    \"maximum_source_bytes\": 1048574,\n"
           << "    \"functions_after_automatic_include\": "
           << functionsAfterAutomaticInclude << ",\n"
           << "    \"maximum_functions\": 1024,\n"
           << "    \"within_known_fixed_limits\": "
           << (withinCompilerLimits ? "true" : "false") << "\n"
           << "  },\n"
           << "  \"warnings\": [";
    for (std::size_t index = 0; index < warnings.size(); ++index) {
        output << (index == 0 ? "" : ", ") << quoteReportString(warnings[index]);
    }
    output << "],\n  \"functions\": [\n";
    for (std::size_t index = 0; index < functions.size(); ++index) {
        const FunctionReport& function = functions[index];
        output << "    {\n"
               << "      \"index\": " << function.index << ",\n"
               << "      \"name\": " << quoteReportString(function.name) << ",\n"
               << "      \"address\": " << function.address << ",\n"
               << "      \"parameter_count\": " << function.parameterCount << ",\n"
               << "      \"call_sign\": " << function.callSign << ",\n"
               << "      \"basic_blocks\": " << function.basicBlockCount << ",\n"
               << "      \"reachable_instructions\": "
               << function.instructionCount << ",\n"
               << "      \"operand_stack_lifted\": "
               << (function.lifted ? "true" : "false") << ",\n"
               << "      \"structured\": "
               << (function.structured ? "true" : "false") << ",\n"
               << "      \"inline_assembly_blocks\": "
               << function.inlineAssemblyCount << ",\n"
               << "      \"residual_gotos\": " << function.gotoCount << ",\n"
               << "      \"output_level\": "
               << quoteReportString(function.outputLevel) << ",\n"
               << "      \"reason\": " << quoteReportString(function.reason) << ",\n"
               << "      \"return_type\": "
               << quoteReportString(valueTypeName(function.returnType)) << "\n"
               << "    }" << (index + 1 == functions.size() ? "\n" : ",\n");
    }
    output << "  ]\n}\n";
    return true;
}

bool SangoEmitter::emit(const Module& module,
                        const std::vector<FunctionGraph>& graphs,
                        const std::vector<LiftedFunction>& functions,
                        const ConfigurationNames&,
                        const TypeAnalyzer& types,
                        const std::string& mode,
                        const std::string& outputPath,
                        DecompilationReport& report,
                        std::string& error) const {
    std::ofstream output(outputPath.c_str());
    if (!output) {
        error = "Unable to create " + outputPath + ".";
        return false;
    }
    output << "// Decompiled from a Sango Smart Script object.\n"
           << "// Assembly fallbacks preserve functions that are not safely liftable.\n\n";

    std::map<std::string, std::vector<ValueType> > externalFunctions;
    for (std::size_t function = 0; function < functions.size(); ++function) {
        for (std::map<std::string, std::vector<ValueType> >::const_iterator
                external = functions[function].externalFunctions.begin();
                external != functions[function].externalFunctions.end(); ++external) {
            if (externalFunctions.find(external->first) == externalFunctions.end()
                    || externalFunctions[external->first].empty()) {
                externalFunctions[external->first] = external->second;
            }
        }
    }
    for (std::map<std::string, std::vector<ValueType> >::const_iterator
            external = externalFunctions.begin();
            external != externalFunctions.end(); ++external) {
        output << "extern void " << sourceIdentifier(external->first, 0) << '(';
        for (std::size_t parameter = 0;
                parameter < external->second.size(); ++parameter) {
            if (parameter != 0) {
                output << ", ";
            }
            output << sourceTypeName(external->second[parameter]);
        }
        output << ");\n";
    }
    if (!externalFunctions.empty()) {
        output << '\n';
    }

    const std::set<int> arrayBases = globalArrayBases(module);
    std::uint32_t slot = 1;
    while (slot <= module.globalCount) {
        if (arrayBases.count(static_cast<int>(slot)) != 0) {
            const int length = globalArrayLength(
                module, arrayBases, static_cast<int>(slot));
            output << sourceTypeName(types.globalType(static_cast<int>(slot)))
                   << " g_var" << slot << '[' << length << "];\n";
            slot += static_cast<std::uint32_t>(length);
            continue;
        }
        const std::uint32_t firstInRun = slot;
        const ValueType runType = types.globalType(static_cast<int>(slot));
        while (slot < module.globalCount
                && arrayBases.count(static_cast<int>(slot + 1)) == 0
                && types.globalType(static_cast<int>(slot + 1)) == runType) {
            ++slot;
        }
        output << sourceTypeName(runType) << ' ';
        for (std::uint32_t item = firstInRun; item <= slot; ++item) {
            if (item != firstInRun) {
                output << ", ";
            }
            output << "g_var" << item;
        }
        output << ";\n";
        ++slot;
    }
    if (module.globalCount != 0) {
        output << '\n';
    }

    report.inputPath = module.inputPath;
    report.outputPath = outputPath;
    report.mode = mode;
    report.decodedInstructionCount = static_cast<int>(module.instructions.size());
    report.unknownWordCount = static_cast<int>(module.unknownWords.size());
    report.warnings = module.warnings;

    std::set<int> cyclicFunctions;
    const std::vector<int> emissionOrder = functionEmissionOrder(
        module, graphs, functions, cyclicFunctions);
    std::vector<int> emissionPosition(module.functions.size(), -1);
    for (std::size_t position = 0; position < emissionOrder.size(); ++position) {
        emissionPosition[emissionOrder[position]] = static_cast<int>(position);
    }
    if (!cyclicFunctions.empty()) {
        report.warnings.push_back(std::to_string(cyclicFunctions.size())
            + " function(s) are in or depend on a recursive definition cycle; "
              "only still-forward callers use assembly.");
    }

    for (std::size_t orderIndex = 0;
            orderIndex < emissionOrder.size(); ++orderIndex) {
        const int index = emissionOrder[orderIndex];
        FunctionReport functionReport;
        functionReport.index = index;
        functionReport.name = module.functions[index].name;
        functionReport.address = module.functions[index].entryAddress;
        functionReport.parameterCount = module.functions[index].parameterCount;
        functionReport.callSign = module.functions[index].callSign;
        functionReport.basicBlockCount = static_cast<int>(graphs[index].blocks.size());
        functionReport.instructionCount
            = static_cast<int>(graphs[index].reachableAddresses.size());
        functionReport.returnType = types.functionReturnType(static_cast<int>(index));
        functionReport.inlineAssemblyCount = functions[index].inlineAssemblyCount;
        for (int parameter = 0;
                parameter < module.functions[index].parameterCount; ++parameter) {
            functionReport.parameterTypes.push_back(
                types.parameterType(static_cast<int>(index), parameter));
        }

        const bool forwardCall = functions[index].succeeded
            && hasUnresolvedForwardCall(module, graphs[index],
                functions[index], emissionPosition);
        const bool useAssembly = mode == "lossless"
            || !functions[index].succeeded || forwardCall;
        if (useAssembly) {
            writeAssemblyFunction(output, module, graphs[index], types);
            functionReport.outputLevel = "naked assembly";
            functionReport.reason = forwardCall
                ? "A recursive dependency still calls a later-defined function."
                : functions[index].failureReason.empty()
                    ? "Lossless mode requested assembly."
                    : functions[index].failureReason;
            ++report.assemblyFunctionCount;
        } else {
            const bool structuredMode = mode != "goto"
                && functions[index].structured;
            writeHighLevelFunction(output, module, graphs[index],
                functions[index], types, structuredMode);
            functionReport.lifted = true;
            functionReport.structured = structuredMode;
            functionReport.gotoCount = structuredMode
                ? functions[index].residualGotoCount
                : std::max(0, static_cast<int>(graphs[index].blocks.size()) - 1);
            functionReport.outputLevel = structuredMode
                ? functions[index].inlineAssemblyCount == 0
                    ? "structured source"
                    : "structured source with inline assembly"
                : "labeled-goto source";
            functionReport.reason = structuredMode
                ? functions[index].inlineAssemblyCount != 0
                    ? "Configuration-signature mismatches use isolated inline assembly."
                : functions[index].usesDispatcher
                    ? "Irreducible residual flow uses a local-state dispatcher."
                    : "Compiler control flow was converted into source regions."
                : "The requested mode retains explicit block control flow.";
            ++report.highLevelFunctionCount;
            if (functions[index].inlineAssemblyCount != 0) {
                ++report.inlineAssemblyFunctionCount;
                report.inlineAssemblyBlockCount
                    += functions[index].inlineAssemblyCount;
            }
            if (structuredMode) {
                ++report.structuredFunctionCount;
                if (functions[index].usesDispatcher) {
                    ++report.dispatcherFunctionCount;
                }
            }
            report.residualGotoCount += functionReport.gotoCount;
        }
        report.functions.push_back(functionReport);
    }
    output.flush();
    const std::ostream::pos_type endPosition = output.tellp();
    if (endPosition >= 0) {
        report.generatedSourceBytes = static_cast<std::uint64_t>(endPosition);
    }
    report.functionsAfterAutomaticInclude
        = static_cast<int>(module.functions.size());
    report.withinCompilerLimits = report.generatedSourceBytes <= 1048574U
        && report.functionsAfterAutomaticInclude <= 1024;
    return true;
}

}  // namespace decompiler
}  // namespace sango
