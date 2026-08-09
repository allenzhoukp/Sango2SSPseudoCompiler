#include "Decompiler.h"

#include <algorithm>
#include <queue>
#include <set>

namespace sango {
namespace decompiler {

namespace {

/** Adds an address once to a traversal queue. */
void queueAddress(std::uint32_t address,
                  std::set<std::uint32_t>& queued,
                  std::queue<std::uint32_t>& pending) {
    if (queued.insert(address).second) {
        pending.push(address);
    }
}

/** Returns the direct branch target of a branch-like instruction. */
std::uint32_t branchTarget(const Instruction& instruction) {
    return instruction.opcode == 0x40
        ? static_cast<std::uint32_t>(instruction.secondOperand)
        : static_cast<std::uint32_t>(instruction.operand);
}

/** Adds one graph edge without duplicating it. */
void addEdge(FunctionGraph& graph, int from, int to) {
    if (from < 0 || to < 0 || from >= static_cast<int>(graph.blocks.size())
            || to >= static_cast<int>(graph.blocks.size())) {
        return;
    }
    std::vector<int>& successors = graph.blocks[from].successors;
    if (std::find(successors.begin(), successors.end(), to) == successors.end()) {
        successors.push_back(to);
    }
    std::vector<int>& predecessors = graph.blocks[to].predecessors;
    if (std::find(predecessors.begin(), predecessors.end(), from)
            == predecessors.end()) {
        predecessors.push_back(from);
    }
}

}  // namespace

FunctionGraph ControlFlowGraphBuilder::build(const Module& module,
                                             int functionIndex) const {
    FunctionGraph graph;
    graph.functionIndex = functionIndex;
    if (functionIndex < 0
            || functionIndex >= static_cast<int>(module.functions.size())) {
        graph.warnings.push_back("The requested function index is invalid.");
        return graph;
    }

    std::queue<std::uint32_t> pending;
    std::set<std::uint32_t> queued;
    queueAddress(module.functions[functionIndex].entryAddress, queued, pending);

    while (!pending.empty()) {
        const std::uint32_t address = pending.front();
        pending.pop();
        std::map<std::uint32_t, Instruction>::const_iterator at
            = module.instructions.find(address);
        if (at == module.instructions.end()) {
            graph.warnings.push_back("Reachable control flow enters undecoded data.");
            continue;
        }
        graph.reachableAddresses.insert(address);
        const Instruction& instruction = at->second;
        if (instruction.opcode == 0x3b) {
            queueAddress(branchTarget(instruction), queued, pending);
        } else if (instruction.opcode == 0x3c || instruction.opcode == 0x3d
                || instruction.opcode == 0x40) {
            queueAddress(branchTarget(instruction), queued, pending);
            queueAddress(instruction.nextAddress(), queued, pending);
        } else if (instruction.opcode != 0x01 && instruction.opcode != 0x02) {
            queueAddress(instruction.nextAddress(), queued, pending);
        }
    }

    std::set<std::uint32_t> leaders;
    leaders.insert(module.functions[functionIndex].entryAddress);
    for (std::set<std::uint32_t>::const_iterator address
            = graph.reachableAddresses.begin();
            address != graph.reachableAddresses.end(); ++address) {
        const Instruction& instruction = module.instructions.find(*address)->second;
        if (instruction.opcode == 0x3b || instruction.opcode == 0x3c
                || instruction.opcode == 0x3d || instruction.opcode == 0x40) {
            leaders.insert(branchTarget(instruction));
        }
        if ((instruction.opcode == 0x3b || instruction.opcode == 0x3c
                || instruction.opcode == 0x3d || instruction.opcode == 0x40
                || instruction.opcode == 0x01 || instruction.opcode == 0x02)
                && graph.reachableAddresses.count(instruction.nextAddress()) != 0) {
            leaders.insert(instruction.nextAddress());
        }
    }

    for (std::set<std::uint32_t>::const_iterator leader = leaders.begin();
            leader != leaders.end(); ++leader) {
        if (graph.reachableAddresses.count(*leader) == 0) {
            graph.warnings.push_back("A direct branch target is not reachable code.");
            continue;
        }
        BasicBlock block;
        block.index = static_cast<int>(graph.blocks.size());
        block.startAddress = *leader;
        std::uint32_t address = *leader;
        while (graph.reachableAddresses.count(address) != 0) {
            if (address != *leader && leaders.count(address) != 0) {
                break;
            }
            const Instruction* instruction
                = &module.instructions.find(address)->second;
            block.instructions.push_back(instruction);
            if (instruction->opcode == 0x01 || instruction->opcode == 0x02
                    || instruction->opcode == 0x3b
                    || instruction->opcode == 0x3c
                    || instruction->opcode == 0x3d
                    || instruction->opcode == 0x40) {
                break;
            }
            address = instruction->nextAddress();
        }
        graph.blockByAddress[block.startAddress] = block.index;
        graph.blocks.push_back(block);
    }

    for (std::size_t index = 0; index < graph.blocks.size(); ++index) {
        BasicBlock& block = graph.blocks[index];
        if (block.instructions.empty()) {
            continue;
        }
        const Instruction& last = *block.instructions.back();
        /** Finds the block beginning at an address or reports no block. */
        const auto findBlock = [&](std::uint32_t address) -> int {
            std::unordered_map<std::uint32_t, int>::const_iterator found
                = graph.blockByAddress.find(address);
            return found == graph.blockByAddress.end() ? -1 : found->second;
        };
        if (last.opcode == 0x3b) {
            addEdge(graph, static_cast<int>(index), findBlock(branchTarget(last)));
        } else if (last.opcode == 0x3c || last.opcode == 0x3d
                || last.opcode == 0x40) {
            addEdge(graph, static_cast<int>(index), findBlock(branchTarget(last)));
            addEdge(graph, static_cast<int>(index), findBlock(last.nextAddress()));
        } else if (last.opcode != 0x01 && last.opcode != 0x02) {
            addEdge(graph, static_cast<int>(index), findBlock(last.nextAddress()));
        }
    }

    for (std::size_t index = 0; index < graph.blocks.size(); ++index) {
        if (graph.blocks[index].instructions.empty()) {
            graph.warnings.push_back("An empty basic block was produced.");
        }
        const BasicBlock& block = graph.blocks[index];
        if (!block.instructions.empty()) {
            const Instruction& last = *block.instructions.back();
            const bool needsTarget = last.opcode == 0x3b || last.opcode == 0x3c
                || last.opcode == 0x3d || last.opcode == 0x40;
            if (needsTarget && block.successors.empty()) {
                graph.warnings.push_back("A branch target does not name a block.");
            }
        }
    }
    return graph;
}

}  // namespace decompiler
}  // namespace sango
