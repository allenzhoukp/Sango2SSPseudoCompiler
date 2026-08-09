#ifndef SANGO_DECOMPILER_H
#define SANGO_DECOMPILER_H

#include <cstdint>
#include <iosfwd>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace sango {
namespace decompiler {

/** Describes the broad source type needed to recompile a stack value. */
enum class ValueType {
    unknown,
    integer,
    floatingPoint,
    string,
    conflict
};

/** Describes how control leaves a lifted basic block. */
enum class ExitKind {
    fallthrough,
    jump,
    branch,
    switchCase,
    returnWithoutValue,
    returnWithValue
};

/** Describes one virtual-machine opcode and its operand count. */
struct InstructionDescription {
    const char* name;
    int operandCount;
};

/** Stores one instruction decoded from the object file. */
struct Instruction {
    std::uint32_t address = 0;
    std::uint32_t opcode = 0;
    std::int32_t operand = 0;
    std::int32_t secondOperand = 0;
    int operandCount = 0;

    /** Returns the byte address immediately after this instruction. */
    std::uint32_t nextAddress() const;
};

/** Stores one function-table entry and its decoded name. */
struct FunctionRecord {
    std::uint32_t entryAddress = 0;
    int parameterCount = 0;
    std::uint32_t nameAddress = 0;
    int callSign = 0;
    std::string name;
};

/** Stores a referenced object string without assuming a text encoding. */
struct ObjectString {
    std::uint32_t address = 0;
    std::uint32_t endAddress = 0;
    std::string bytes;
};

/** Stores the lossless facts needed by the decompiler and assembly fallback. */
struct Module {
    std::string inputPath;
    std::vector<unsigned char> bytes;
    std::uint32_t version = 0;
    std::uint32_t codeStart = 0;
    std::uint32_t functionTableAddress = 0;
    std::uint32_t stringTableAddress = 0;
    std::uint32_t globalCount = 0;
    std::uint32_t logicalEnd = 0;
    std::vector<FunctionRecord> functions;
    std::vector<std::uint32_t> runtimeStringAddresses;
    std::map<std::uint32_t, ObjectString> strings;
    std::map<std::uint32_t, Instruction> instructions;
    std::set<std::uint32_t> unknownWords;
    std::vector<std::string> warnings;
};

/** Stores a configured system call used to give emitted calls readable names. */
struct SystemCall {
    std::uint32_t number = 0;
    int parameterCount = 0;
    ValueType returnType = ValueType::unknown;
    std::string name;
};

/** Stores optional configuration names used by source emission. */
struct ConfigurationNames {
    std::unordered_map<std::uint32_t, SystemCall> systemCalls;
    std::unordered_map<std::int32_t, std::string> internalValues;
    std::vector<std::string> warnings;

    /** Loads system-call and engine-value names from the repository configuration. */
    void load(const std::string& configurationDirectory);
};

/** Stores one maximal straight-line instruction sequence. */
struct BasicBlock {
    int index = -1;
    std::uint32_t startAddress = 0;
    std::vector<const Instruction*> instructions;
    std::vector<int> successors;
    std::vector<int> predecessors;
};

/** Stores the reachable control-flow graph for one function entry. */
struct FunctionGraph {
    int functionIndex = -1;
    std::vector<BasicBlock> blocks;
    std::unordered_map<std::uint32_t, int> blockByAddress;
    std::set<std::uint32_t> reachableAddresses;
    std::vector<std::string> warnings;

    /** Returns true when every direct control-flow target belongs to a block. */
    bool isComplete() const;
};

/** Stores a source expression together with the facts needed for safe emission. */
struct Expression {
    std::string text;
    ValueType type = ValueType::unknown;
    bool hasSideEffect = false;

    /** Returns true when two incoming stack values can be merged without a temporary. */
    bool sameAs(const Expression& other) const;
};

/** Stores a block after operand-stack instructions have become source statements. */
struct LiftedBlock {
    int index = -1;
    std::uint32_t address = 0;
    std::vector<std::string> statements;
    ExitKind exitKind = ExitKind::fallthrough;
    std::string condition;
    std::string returnValue;
    std::int32_t switchValue = 0;
    int target = -1;
    int fallthrough = -1;
};

/** Stores all high-level facts recovered for one function. */
struct LiftedFunction {
    int functionIndex = -1;
    bool succeeded = false;
    bool structured = false;
    bool usesDispatcher = false;
    bool keepsSwitchWithGotos = false;
    bool returnsValue = false;
    std::string failureReason;
    ValueType returnType = ValueType::unknown;
    std::vector<ValueType> parameterTypes;
    std::map<int, ValueType> localTypes;
    std::set<int> localArrayBases;
    std::map<std::string, std::vector<ValueType> > externalFunctions;
    std::vector<LiftedBlock> blocks;
    std::vector<std::string> structuredBody;
    int inlineAssemblyCount = 0;
    int residualGotoCount = 0;
};

/** Stores the machine-readable result for one emitted function. */
struct FunctionReport {
    int index = -1;
    std::string name;
    std::uint32_t address = 0;
    int parameterCount = 0;
    int callSign = 0;
    int basicBlockCount = 0;
    int instructionCount = 0;
    bool lifted = false;
    bool structured = false;
    int inlineAssemblyCount = 0;
    int gotoCount = 0;
    std::string outputLevel;
    std::string reason;
    ValueType returnType = ValueType::unknown;
    std::vector<ValueType> parameterTypes;
};

/** Stores summary and per-function details written beside generated source. */
struct DecompilationReport {
    std::string inputPath;
    std::string outputPath;
    std::string mode;
    int decodedInstructionCount = 0;
    int unknownWordCount = 0;
    int highLevelFunctionCount = 0;
    int structuredFunctionCount = 0;
    int assemblyFunctionCount = 0;
    int inlineAssemblyFunctionCount = 0;
    int inlineAssemblyBlockCount = 0;
    int omittedIncludeFunctionCount = 0;
    int dispatcherFunctionCount = 0;
    int residualGotoCount = 0;
    std::uint64_t generatedSourceBytes = 0;
    int functionsAfterAutomaticInclude = 0;
    bool withinCompilerLimits = false;
    std::vector<std::string> warnings;
    std::vector<FunctionReport> functions;

    /** Writes this report as readable JavaScript Object Notation. */
    bool write(const std::string& path, std::string& error) const;
};

/** Decodes Smart Script objects without inferring high-level program meaning. */
class ObjectDecoder {
public:
    /** Reads, validates, and conservatively decodes one object file. */
    bool decode(const std::string& path, Module& module, std::string& error) const;
};

/** Builds basic blocks and typed control-flow edges for each function. */
class ControlFlowGraphBuilder {
public:
    /** Builds a graph from one function entry without following call edges. */
    FunctionGraph build(const Module& module, int functionIndex) const;
};

/** Infers compiler-acceptable parameter, local, and return types. */
class TypeAnalyzer {
public:
    /** Solves type constraints across all function graphs and direct calls. */
    void analyze(const Module& module,
                 const std::vector<FunctionGraph>& graphs,
                 const ConfigurationNames& names);

    /** Returns the inferred return type for a function. */
    ValueType functionReturnType(int functionIndex) const;

    /** Returns true when a function executes a value-return instruction. */
    bool functionReturnsValue(int functionIndex) const;

    /** Returns the inferred type for one function parameter. */
    ValueType parameterType(int functionIndex, int parameterIndex) const;

    /** Returns the inferred type for one local stack slot. */
    ValueType localType(int functionIndex, int slot) const;

    /** Returns the inferred type for one global stack slot. */
    ValueType globalType(int slot) const;

    /** Returns the inferred result type of a value-producing instruction. */
    ValueType instructionType(std::uint32_t address) const;

private:
    struct TypeNode {
        int parent = -1;
        ValueType type = ValueType::unknown;
    };

    std::vector<TypeNode> nodes_;
    std::vector<int> returnNodes_;
    std::vector<bool> returnsValue_;
    std::vector<std::vector<int> > parameterNodes_;
    std::vector<std::map<int, int> > localNodes_;
    std::map<int, int> globalNodes_;
    std::map<std::uint32_t, int> instructionNodes_;

    /** Creates one initially unconstrained type node. */
    int makeNode();

    /** Returns the representative of a type node and shortens its parent chain. */
    int findNode(int node);

    /** Applies a concrete type requirement to one node. */
    void requireType(int node, ValueType type);

    /** Makes two values share one inferred type. */
    void joinNodes(int left, int right);

    /** Returns a node for a local slot, creating it when first referenced. */
    int localNode(int functionIndex, int slot);

    /** Returns a node for a global slot, creating it when first referenced. */
    int globalNode(int slot);

    /** Returns a stable result node for a value-producing instruction. */
    int instructionNode(const Instruction& instruction);

    /** Returns the resolved type of a node without changing public state. */
    ValueType resolvedType(int node) const;
};

/** Converts stack instructions into expressions and ordered source statements. */
class StackLifter {
public:
    /** Lifts one function while retaining its original block-level control flow. */
    LiftedFunction lift(const Module& module,
                        const FunctionGraph& graph,
                        const ConfigurationNames& names,
                        const TypeAnalyzer& types) const;
};

/** Converts reducible compiler control flow into nested source constructs. */
class PatternStructurer {
public:
    /** Structures one lifted function and leaves explicit gotos only when needed. */
    void structure(const FunctionGraph& graph, LiftedFunction& function) const;
};

/** Emits compilable Sango source or a readable naked-assembly fallback. */
class SangoEmitter {
public:
    /** Writes all functions and fills the corresponding report records. */
    bool emit(const Module& module,
              const std::vector<FunctionGraph>& graphs,
              const std::vector<LiftedFunction>& functions,
              const ConfigurationNames& names,
              const TypeAnalyzer& types,
              const std::string& mode,
              const std::string& outputPath,
              DecompilationReport& report,
              std::string& error) const;
};

/** Returns the description of a known virtual-machine opcode. */
const InstructionDescription* instructionDescription(std::uint32_t opcode);

/** Returns a readable spelling for one inferred value type. */
const char* valueTypeName(ValueType type);

/** Returns the Sango source spelling used for one inferred value type. */
const char* sourceTypeName(ValueType type);

/** Escapes arbitrary object bytes as one compiler-readable string literal. */
std::string quoteSourceString(const std::string& bytes);

/** Converts a function-table name into a legal and stable source identifier. */
std::string sourceIdentifier(const std::string& name, int fallbackIndex);

}  // namespace decompiler
}  // namespace sango

#endif
