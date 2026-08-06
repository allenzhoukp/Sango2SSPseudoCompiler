# Sango Smart Script Decompiler Plan

## 1. Goal

Build a decompiler for Sango Smart Script object files (`.so`) that emits
source accepted by the existing `ss.exe` compiler and preserves the original
program's behavior.

Readable structured source is the eventual goal, but it is not the first
correctness milestone. The project should progress through four increasingly
high-level representations:

```text
exact reassemblable assembly
    -> compilable source containing naked/inline assembly
    -> high-level expressions and statements with labels/gotos
    -> structured high-level source with if/loop/switch constructs
```

Each level remains a fallback for the level above it. A function that cannot
yet be expressed safely at a high level should still be emitted in a form that
compiles, rather than being guessed or silently omitted.

### Definition of success

For the supported instruction set:

1. The decompiler needs no hand editing.
2. Its output compiles with the original `ss.exe`.
3. The recompiled program has equivalent VM-visible behavior.
4. Unsupported or ambiguous regions are preserved with an explicit assembly
   fallback.
5. Structuralization improves readability but never becomes a prerequisite
   for correct output.

Byte-for-byte equality is useful for the assembly preservation path, but it is
not required for high-level output. Function ordering, addresses, strings,
labels, compiler-generated jumps, and provenance trailers may legitimately
change while behavior remains the same.

## 2. Guiding principles

### Correctness before prettiness

The first useful decompiler should emit boring, globally unique labels and
gotos. This already removes most stack-machine syntax and gives editable,
recompilable source. Structured `if`, loop, and switch recovery is a later
transformation of an already correct program.

### Keep lifting and structuralization separate

The old `SSAsm2Pseudo` project had the right architectural instinct: recover
the real expressions and statements independently from the CFG topology. Its
surrogate-x86/Hex-Rays stage should be replaced by a direct CFG-to-structure
adapter, not reproduced with another native decompiler.

### Model the VM globally, not as a linear instruction stream

Compiler-generated expressions are postfix/RPN stack programs: operands are
emitted first and the operator follows. Linear stack simulation works inside a
straight-line block, but fails at joins, loops, retained switch selectors,
spills, and shared tails. The lifter must propagate symbolic stack states
through the CFG until they reach a fixed point.

### Preserve side effects and evaluation order

Calls, syscalls, stores, memory accesses, and configured INTV operations must
remain explicit until it is proven safe to embed them in a larger expression.
No C-oriented tool may be allowed to reorder or duplicate them accidentally.

### Make failure visible and recoverable

Every lift or rewrite should be able to explain why it was rejected: unknown
opcode, inconsistent stack height, conflicting type constraints, unsupported
call graph, unsafe condition rewrite, or failed round-trip verification.

## 3. Existing foundations

The repository already contains most of the format-specific knowledge needed
for a real front end:

- [`cc/Sg2dasm2.cpp`](cc/Sg2dasm2.cpp) conservatively discovers code from all
  function entries, follows direct control flow, avoids referenced strings,
  and preserves undecoded data.
- [`cc/Sg2dasm2.h`](cc/Sg2dasm2.h) contains the known opcode and operand-count
  table.
- [`magic_inst.txt`](magic_inst.txt) documents VM instruction behavior.
- [`fullauto-docs/so-file-format.md`](fullauto-docs/so-file-format.md) documents
  the object layout and its edge cases.
- [`fullauto-docs/grammar-guide.md`](fullauto-docs/grammar-guide.md) describes
  the actual source language accepted by the compiler.
- [`cc/TreeAnalysis.cpp`](cc/TreeAnalysis.cpp) is an executable description of
  how source expressions are lowered to stack instructions.
- [`cc/BranchAnalysis.cpp`](cc/BranchAnalysis.cpp) shows the exact compiler
  patterns for `if`, loops, `for`, `switch`, calls, labels, and assembly.
- The old `SSAsm2Pseudo` input/goto/structured outputs provide a large
  historical comparison corpus.

The current disassembler should be refactored into reusable components rather
than parsed again through its textual assembly output.

## 4. Proposed pipeline

```text
.so object
   |
   v
object decoder and conservative code discovery
   |
   v
per-function control-flow graphs
   |
   v
stack-to-SSA/data-flow lifting
   |
   +----> explicit side-effect statements
   +----> expression DAGs
   +----> type and signature constraints
   |
   v
correct labeled-goto Sango AST  <---- semantic baseline
   |
   v
compiler-pattern and region structuralization
   |
   v
Sango-aware source emitter
   |
   v
ss.exe -> re-lift -> verify
```

The internal AST must be owned by this project. External structuralizers may
produce or suggest regions, but they must not define the semantic IR or source
language rules.

## 5. Core intermediate representations

### Object/module IR

This layer represents only facts read from the object:

- header fields and preserved raw bytes;
- strings and their original byte encodings;
- function-table records;
- decoded instructions with original addresses and raw operands;
- referenced code/data ranges;
- undecoded or conflicting ranges;
- compiler/assembler trailer data.

It should not yet infer expressions, variables, or structured control flow.

### Function CFG IR

A function graph contains:

- basic blocks and original address ranges;
- fallthrough, conditional, switch, call, and return edges;
- a synthetic exit where useful;
- ownership/reachability from each function entry;
- support for shared tails or overlapping entry reachability;
- source-address provenance on every block and instruction.

Function boundaries must not be guessed merely from function-table order,
because the object table does not store function sizes.

### Lifted semantic IR

Use an SSA-like value graph with explicit side effects:

- constants and string references;
- local, parameter, global, and INTV reads;
- arithmetic, comparison, conversion, and address operations;
- calls and syscalls with explicit arguments and optional results;
- stores and other state-changing operations;
- terminators (`jump`, conditional jump, switch dispatch, return);
- an ordered symbolic stack at each block entry and exit;
- phi-like block parameters for values merged at control-flow joins.

This need not be textbook LLVM SSA. The important properties are that each
symbolic value has one definition, joins are explicit, and the operand stack is
part of the data-flow state.

### Structural AST

Keep this deliberately small:

- sequence;
- `if`/`else`;
- loop;
- switch/case;
- break/continue;
- label/goto;
- return;
- optional dispatcher node for an external structurizer's irreducible-CFG
  fallback.

Expressions and side-effect statements should be referenced from the lifted
IR rather than reconstructed independently by the structuralizer.

## 6. Milestones

### Milestone 0: Establish the test oracle

Purpose: make every later transformation measurable.

Work:

1. Record reproducible commands for disassembly, assembly, compilation, and
   re-disassembly.
2. Select a small fixture for fast development and several larger fixtures for
   coverage.
3. Verify the existing exact disassembler/assembler preservation path.
4. Normalize nondeterministic compiler text such as timestamps in comparisons.
5. Create a machine-readable per-run report rather than relying only on visual
   diffs.

Large proprietary or historical binary fixtures do not need to be committed.
The harness can accept external fixture paths, while tiny synthetic fixtures
can be generated from source.

Exit criteria:

- one command exercises the current `.so -> asm -> .so` path;
- failures identify the first differing region or decoded instruction;
- the selected baseline fixtures have recorded expected results.

Compiler concepts exercised: lexical decoding, serialization, golden tests,
and reproducible builds.

### Milestone 1: Refactor the object decoder

Purpose: make the current disassembler usable by both assembly and decompiler
front ends.

Work:

1. Separate binary reading, object validation, code discovery, symbol naming,
   and text emission.
2. Return a `Module`/object IR instead of keeping all state inside `main`.
3. Preserve every original byte and address even when a region is not decoded.
4. Keep the existing conservative rule that an unknown opcode stops that
   discovery path.
5. Retain current assembly output as a regression consumer of the new decoder.

Exit criteria:

- current assembly output remains equivalent;
- the same decoded module can be consumed without reparsing text;
- malformed offsets and overlapping code/data produce diagnostics, not memory
  errors.

Compiler concepts exercised: front-end separation and lossless IR design.

### Milestone 2: Construct per-function CFGs

Purpose: turn decoded addresses into explicit control flow.

Work:

1. Identify leaders: function entries, branch targets, and instructions after
   terminators/conditional branches.
2. Form maximal basic blocks.
3. Add typed CFG edges for fallthrough, conditional targets, switch cases,
   jumps, calls, and returns.
4. Compute reachability separately for each function entry.
5. Represent shared tails instead of arbitrarily assigning them to the first
   function visited.
6. Add optional DOT/JSON output for inspection and structuralizer adapters.

Exit criteria:

- every reachable decoded instruction belongs to at least one function graph;
- all direct targets land at block boundaries;
- CFG visualization agrees with representative disassembly by inspection;
- invalid targets remain explicit diagnostics/fallbacks.

Compiler concepts exercised: basic blocks, CFGs, graph traversal, dominators,
and strongly connected components.

### Milestone 3: Lift the operand stack to SSA-like values

Purpose: recover expressions and statement boundaries correctly across control
flow.

Work:

1. Define the exact pop/push behavior and side effects of every known opcode.
2. Symbolically execute each block from an incoming ordered stack vector.
3. Propagate outgoing stacks to successors with a worklist algorithm.
4. At joins, require compatible stack heights and create one phi/block argument
   per differing stack position.
5. Iterate loops until incoming values stabilize.
6. Handle `JCOND` specially because the switch selector remains on the stack.
7. Obtain direct-call arity from the function table and syscall arity from the
   packed syscall operand.
8. Keep stores, calls, syscalls, and other effects as ordered statements.
9. Build expression DAGs only within the constraints imposed by those effects.

If stack heights disagree at a join, do not invent a value. Mark the affected
region/function for assembly fallback and include the predecessor states in the
diagnostic.

Exit criteria:

- straight-line arithmetic, calls, branches, loops, and switches all lift;
- stack height is known at every reachable block boundary;
- expression output is stable regardless of block visitation order;
- the old linear-lifter corpus no longer depends on hand-adjusted input.

Compiler concepts exercised: abstract interpretation, fixed-point data-flow,
SSA, phi nodes, and dominance.

### Milestone 4: Infer types, variables, and signatures

Purpose: emit source that the compiler's type checker accepts without making
unsafe guesses.

Start with a small type lattice:

```text
unknown
  |-- integer/raw32/address
  |-- float
  |-- string
  `-- conflict
```

The integer/raw-address classes can initially share source representation
because the compiler intentionally treats integer widths and pointers as a
broad compatibility family. Preserve uncertainty in the IR even when the
first emitter spells it `int`.

Constraints come from:

- integer, float, and string opcode variants;
- conversions;
- `PUSHSTR`;
- configured syscall signatures and return categories;
- all direct call sites;
- return instructions and returned values;
- loads/stores of the same VM slot;
- configured INTV and structure information.

Run function parameter and return inference to a call-graph fixed point. Avoid
assigning attractive source types such as arrays or structures until the
evidence is strong; those are readability improvements, not requirements for
the first recompilable output.

Exit criteria:

- every emitted expression has a compiler-acceptable type or a documented
  fallback;
- conflicts name all contributing uses;
- signatures stabilize across the call graph;
- raw float bit patterns and ambiguous pointer/integer values remain
  representable.

Compiler concepts exercised: constraint solving, type lattices, unification,
and interprocedural analysis.

### Milestone 5: Emit correct labeled-goto Sango source

Purpose: deliver the first true decompiler MVP.

Work:

1. Declare parameters and VM local slots with stable generated names.
2. Initially access globals by stable numeric helpers when that better
   preserves original slot numbering.
3. Emit one statement sequence per basic block.
4. Emit globally unique labels such as `fn_004_L_0012`.
5. Parenthesize every nontrivial expression; Sango precedence is not C
   precedence.
6. Preserve original evaluation order around calls and effects by introducing
   temporaries where necessary.
7. End blocks with explicit `if (...) goto`, `goto`, or `return`.
8. Emit metadata such as function names and callsigns.
9. Produce a sidecar report listing inferred types, fallbacks, and verification
   status.

Special handling for the unchanged `ss.exe`:

- Internal calls to later-defined functions are not accepted.
- An acyclic call graph may be topologically ordered only if changing function
  table order is known to be safe.
- Direct self-recursion is supported.
- Mutual recursion or order-sensitive functions require a stack-neutral inline
  assembly bridge or whole-function naked assembly fallback unless compiler
  changes are explicitly permitted later.

Exit criteria:

- representative objects decompile and recompile with no manual edits;
- every function is emitted, even if some use naked assembly;
- re-lifted output passes the initial equivalence checks;
- the report clearly separates high-level, inline-assembly, and naked-assembly
  coverage.

Compiler concepts exercised: code generation, pretty-printing, symbol tables,
and calling conventions.

### Milestone 6: Recover known compiler structures

Purpose: improve readability using facts specific to this compiler.

Perform compiler-aware transformations first:

1. Recognize the exact `if` and `if/else` branch layouts produced by
   `BranchAnalysis.cpp`.
2. Recover `while` and `do/while` loops from their known header/tail tests.
3. Recover `for` only when initializer, condition, increment, continue target,
   and body are unambiguous; otherwise prefer `while`.
4. Recognize the retained-selector `JCOND` chain as a switch.
5. Convert exits to break/continue where doing so reduces gotos without
   changing their target.
6. Use dominators, postdominators, natural loops, and single-entry/single-exit
   regions for ordinary cases not matched by exact templates.
7. Leave all residual or apparently intentional edges as gotos.

Never combine branch regions into `a && b` or `a || b` merely because a
C-oriented structurer does so. Sango evaluates both logical operands rather
than short-circuiting them. Nested conditions, explicit temporaries, or the
original gotos must preserve the VM evaluation order.

Exit criteria:

- canonical compiler-generated examples recover their corresponding source
  construct;
- every structured AST can be lowered back to the validated goto CFG;
- accepting a rewrite cannot reduce verification status;
- residual gotos remain legal and readable.

Compiler concepts exercised: dominators/postdominators, natural loops, region
analysis, and inverse compiler transformations.

### Milestone 7: Evaluate open-source structuralizers

Purpose: decide whether a maintained toolbox materially improves difficult
graphs without becoming the semantic foundation of the project.

Prototype direct adapters for representative CFGs:

#### angr SAILR

- Project: <https://github.com/angr/angr>
- Best candidate for decompiler-style readable regions.
- It recursively applies compiler-aware structuring and preserves some gotos
  rather than forcing all graphs into dispatchers.
- Integration requires adapting this project's blocks and conditions to angr
  AIL/region structures; use it first as a Python sidecar experiment.

#### Binaryen Relooper

- Project: <https://github.com/WebAssembly/binaryen>
- C++ implementation designed to turn a general CFG into WebAssembly-like
  block/loop/if/branch shapes.
- Strong candidate for deterministic coverage of arbitrary and irreducible
  graphs.
- Conditions supplied to it must be pure temporary reads because it may
  rearrange or eliminate tests.
- Its irreducible fallback may be a selector-variable dispatcher; retain the
  original goto form whenever that is clearer.

#### MLIR CFG-to-SCF

- Project: <https://mlir.llvm.org/docs/Passes/#-lift-cf-to-scf>
- Useful modern reference and possible experiment for general CFG-to-structured
  conversion.
- Do not make LLVM/MLIR a release dependency unless its output proves
  substantially better than the smaller alternatives.

Evaluate each result using:

- recompilation success;
- behavioral/canonical-IR verification;
- remaining gotos;
- duplicated statements or conditions;
- introduced selector variables;
- nesting depth;
- emitted source size;
- dependency/build cost;
- Sango-emittability without Boolean semantic changes.

Expected production direction:

```text
native compiler-pattern recovery
    + readable residual gotos
    + optional SAILR pass if it wins on the real corpus
    + Relooper only when its structured fallback is clearer
```

Compiler concepts exercised: IR adapters, region trees, irreducible flow, and
cost models.

### Milestone 8: Strengthen equivalence testing

Purpose: distinguish “it compiled” from “it retained behavior.”

Use several layers of evidence:

1. **Exact binary comparison** for the assembly-preservation mode.
2. **Decoded instruction/action comparison** for code expected to lower into
   the same compiler patterns.
3. **Canonical lifted-IR comparison** after normalizing labels, addresses,
   temporary slots, condition inversion, and harmless block layout changes.
4. **Differential VM execution** with identical inputs and deterministic
   syscall stubs, comparing returns, globals, INTV writes, memory writes, and
   syscall traces.
5. **Generated-source testing**: compile small programs exercising every
   language construct and opcode family, decompile them, recompile, and compare.
6. **Fuzz/property testing** for valid stack programs within a constrained
   opcode subset.

An interpreter need not implement the entire game. A useful verification VM
can treat syscalls as typed, trace-producing external operations and compare
the ordered call traces between original and regenerated functions.

Exit criteria:

- all accepted structural rewrites pass the same checks as goto output;
- verification failures automatically retain or restore the lower-level form;
- corpus reports make regressions visible per function and opcode.

Compiler concepts exercised: interpreters, semantic normalization,
differential testing, and property-based testing.

### Milestone 9: Optional readability improvements

These should not delay the recompilable MVP:

- better variable and function names;
- array and structure-member recovery;
- pointer idiom recognition;
- compound assignment and increment recovery;
- `for` preference heuristics;
- common expression and temporary cleanup;
- source comments containing original addresses and uncertainty;
- an interactive CFG/source view;
- a Ghidra loader/SLEIGH processor if interactive reverse engineering becomes
  a separate product goal.

## 7. Fallback policy

Choose the highest safe representation independently for each function or
region:

| Level | Output | Use when |
| --- | --- | --- |
| A | Structured source | Structure and expression rewrites verify |
| B | High-level statements plus gotos | Lifting verifies but structuring does not |
| C | Inline `__asm` | A stack-neutral region cannot be expressed safely in source |
| D | Whole-function naked `__asm` | Stack/type/call constraints prevent mixed output |
| E | Exact reassemblable `.asm` | Unknown instruction shape or object-layout issue prevents source wrapping |

Fallback is a coverage mechanism, not a hidden failure. The decompiler report
must say which level was selected and why.

## 8. Sango-specific hazards checklist

Every emitter or external adapter must account for these rules:

- Source labels are translation-unit-global, not function-local.
- Later internal functions cannot normally be called by earlier source.
- `&&` and `||` are eager rather than short-circuiting.
- Operator precedence and associativity differ from C.
- Locals have function scope.
- Empty statements and empty `for` clauses are not generally available.
- `default` must be emitted last in a switch.
- Normal functions receive compiler-generated stack setup and return code;
  naked functions do not.
- Inline assembly rewriting knows local/parameter/INTV names but is not a real
  assembly parser.
- The function table loses parameter types, return types, local types, local
  names, function sizes, and most high-level aggregate information.
- Code and string data share a section.
- Unknown future opcodes cannot be safely skipped without knowing their operand
  count.
- Configuration files are part of the language ABI and must be recorded in a
  decompilation report.

## 9. Suggested implementation shape

Keep the release core in C++ so it can reuse the current decoder without a
textual or foreign-process boundary. External research adapters may remain
optional Python/tools.

Possible layout:

```text
cc/decompiler/
    ObjectModel.*
    ObjectDecoder.*
    CodeDiscovery.*
    ControlFlowGraph.*
    StackLifter.*
    SemanticIR.*
    TypeInference.*
    SangoAST.*
    PatternStructurer.*
    SangoEmitter.*
    VerificationReport.*

cc/Sg2dasm2.cpp       assembly CLI using the shared decoder
cc/Sg2decomp.cpp      decompiler CLI
```

Illustrative CLI:

```text
sg2decomp.exe input.so -o output.cpp --mode=structured --report output.json
sg2decomp.exe input.so -o output.cpp --mode=goto
sg2decomp.exe input.so -o output.cpp --mode=lossless
```

Do not commit large generated assembly, object, or corpus outputs. Tests should
generate temporary output outside the source tree or under an ignored build
directory.

## 10. Progress metrics

Record these per corpus and per run:

- object decode success;
- decoded instruction and opcode coverage;
- functions with valid CFGs;
- functions with consistent stack data flow;
- functions with stable signatures/types;
- functions emitted as structured, goto, inline assembly, or naked assembly;
- source compilation success;
- exact/canonical/differential verification status;
- number of residual gotos;
- nesting depth and source size;
- number of manual edits required (target: zero).

These metrics prevent a prettier output from hiding a loss of correctness or
coverage.

## 11. Open design decisions

Resolve these with small experiments rather than committing early:

1. Whether to represent stack joins as conventional phi nodes or explicit
   block arguments.
2. Whether shared function tails should be duplicated in source, retained as
   cross-function assembly flow, or assigned to a canonical owner.
3. Whether preserving original function-table order is required by any game
   behavior outside documented callsigns/names.
4. Whether compiler changes such as internal forward declarations are allowed;
   the baseline plan assumes the original `ss.exe` is unchanged.
5. How much canonical IR normalization is sufficient before a verification VM
   becomes necessary.
6. Whether SAILR produces enough improvement to justify an optional Python
   dependency or sidecar tool.
7. Whether unknown opcode support should accept a user-supplied operand/stack
   specification.

## 12. Recommended first vertical slice

Do not begin with structuralization. Implement one complete, narrow path:

1. Decode one small `.so` through the shared object model.
2. Build CFGs for all its functions.
3. Lift a restricted set of stack operations, calls, stores, branches, and
   returns.
4. Emit aggressively parenthesized high-level statements with unique labels
   and gotos.
5. Use naked assembly for any function outside that subset.
6. Compile the entire generated source with the original `ss.exe`.
7. Re-lift the result and produce a per-function comparison report.

Once this works without hand editing, add opcode families and type constraints
until goto-mode coverage is high. Only then begin the structuralizer bake-off.

That vertical slice exercises every essential interface while keeping each
individual compiler algorithm small enough to understand and debug.
