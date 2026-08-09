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
3. The recompiled program has equivalent behavior as observed by the virtual
   machine.
4. Unsupported or ambiguous regions are preserved with an explicit assembly
   fallback.
5. Structuralization improves readability but never becomes a prerequisite
   for correct output.

Byte-for-byte equality is useful for the assembly preservation path, but it is
not required for high-level output. Function ordering, addresses, strings,
labels, compiler-generated jumps, and provenance trailers may legitimately
change while behavior remains the same.

The primary structural sanity check is the existing manual decompilation from
[`magic.so`](../magic.so) to
[`magic_structured.cpp`](../magic_structured.cpp). That source has been tested
many times as a behavior-preserving reference. Automatically generated source
does not need to match it character for character, but its basic function,
branch, loop, and switch structure should be recognizably the same. Behavioral
verification remains authoritative if harmless source-layout choices differ.

### Starting implementation state

An older experimental implementation existed under `cc/decompiler`, but its
design and results were not reliable. The production implementation described
by this plan must start from scratch rather than repair, copy, or extend that
experiment. Old code or output may be inspected only as comparison material;
it is not a correctness oracle. Independently verified repository components,
such as the lossless object decoder and assembly emitter, may still be
refactored into shared code.

The new command-line interface described in section 9 should be implemented as
`sg2decomp.exe`. Its report should record rather than reject output that
exceeds two fixed limits in the unchanged compiler: its main source-file
buffer holds at most 1,048,574 bytes and its function table holds 1,024
functions. Such output remains useful for reading and for a compiler rebuilt
with larger arrays.

On Windows Subsystem for Linux, the preservation and decompilation paths can
use the Windows assembler through the subsystem's Windows interoperability
bridge. The test should verify byte-identical reassembly first, then run object
to decompiled source to compiler assembly to object and check that the
regenerated object can be lifted with the same function interfaces.

### Implemented from-scratch vertical slice

The new implementation now lives in [`cc/decompiler`](../cc/decompiler) with
[`cc/Sg2decomp.cpp`](../cc/Sg2decomp.cpp) as its command-line entry point. It
was written from scratch after the old experiment was discarded. It includes:

- lossless object decoding and conservative instruction discovery;
- a separate per-function control-flow graph builder;
- cross-function parameter, local, global, return, and instruction-result type
  constraints;
- block-aware operand-stack lifting modeled on the readable statement ideas in
  the working part of the old `Stmt.py`;
- natural-loop, conditional-region, and switch-chain recovery;
- a local-state `while`/`switch` dispatcher for residual irreducible flow, so
  difficult functions remain compilable without source `goto` statements;
- stable dependency ordering that moves only the functions needed to place a
  same-object callee before its caller;
- recovered local and global array declarations plus indexed local, global,
  and configured engine-value reads and writes;
- external declarations for cross-object asynchronous calls, including
  `InitializeBattle()` in the system object;
- small inline assembly statements for isolated void system calls whose object
  signature disagrees with the selected configuration, instead of converting
  the containing function to naked assembly;
- readable whole-function naked assembly when an instruction or stack merge
  still cannot be represented safely;
- structured, labeled-goto, and lossless command modes plus a machine-readable
  report with compiler-limit preflight data.

Array recovery exposed an off-by-one allocation error in the compiler: the
named array variable already occupies element zero, but the compiler formerly
reserved the full declared length after it. Local and global declarations now
reserve only the remaining elements, keeping later physical slot numbers
stable.

[`tests/decompiler/roundtrip-windows-interop.sh`](../tests/decompiler/roundtrip-windows-interop.sh)
builds the tool and tests every object fixture. Thirteen objects fit the unchanged
compiler and pass object to source to compiler assembly to Windows assembler to
object to decompiler. Every structured output stays below ten residual source
gotos.
`Sango7Script.so` exceeds both unchanged-compiler limits, so the test records
that preflight result and verifies its exact assembly path byte for byte.

Their structured output has zero residual source gotos.

For the primary `magic.so` sanity check, all 454 object-owned functions are
emitted as high-level structured source without local-state dispatchers, naked
assembly, or inline assembly, and the nine
`sg2lang.h` functions are supplied by the compiler. The generated globals and
locals recover the important array bases and lengths seen in the manual
reference. The output has zero residual source gotos, compiles, assembles, and
can be decompiled again with the same function interfaces.

[`tests/decompiler/compare-structure.py`](../tests/decompiler/compare-structure.py)
compares every generated function with `magic_structured.cpp` using coarse
`if`, `while`, and `switch` counts. The current output matches those counts for
414 of 454 functions; the remaining 40 are the focused readability-review
list. This comparison is a review aid, not
an equivalence proof: different construct counts can still describe the same
behavior, and matching counts do not prove that statements are nested under
the correct condition.

The original `System.SO` is the second primary sanity check. All 29 functions
are high-level structured source without a local-state dispatcher, and none
uses naked or inline assembly. Its cross-object `InitializeBattle()` call uses
an external declaration. The generated source compiles, assembles, and can be
decompiled again with all 29 original function interfaces preserved; the
compiler also supplies nine `sg2lang.h` helper functions in the regenerated
object.

The distinct, newer `SYSTEM_SOMENEW.SO` fixture has all 31 functions emitted
at high level and the same cross-object
`InitializeBattle()` call uses an external declaration. `WJJR()` retains three
small inline assembly statements because that object gives system calls
`0x213`, `0x214`, and `0x215` signatures that disagree with the selected
configuration. No whole function uses naked assembly.

[`tests/decompiler/audit-memory-operations.py`](../tests/decompiler/audit-memory-operations.py)
compares structured and lossless output for every fixture. Eleven objects use
configured memory system calls or the older `sg2lang.h` helpers, and all retain
a one-to-one count: lifted operations become named high-level calls, while
operations in assembly fallback remain explicit. Two retained-selector switch
chains in `system_dudu_using.so` previously expanded one `GetShort` into four
case comparisons and one discarded-expression call. Recognizing the trailing
`POP` as switch bookkeeping now emits one selector call. The make target runs
the audit with `--require-exact-counts` so either lost or duplicated operations
fail the test.

`Sango7Script.so` has extensive same-object interdependencies. Before function
reordering, 240 otherwise liftable functions required assembly solely because
they called a later definition. Stable dependency ordering resolves all of
those ordering failures and reports no recursive definition cycle. Its exact
assembly preservation path remains byte-identical; much of the object still
requires assembly because its system-call/instruction dialect is not described
by the current configuration.

### Current known limitations

- Array length is not stored in the object. The decompiler infers it from the
  next independently addressed local/global slot; an adjacent unused scalar is
  inherently ambiguous and may be folded into the preceding array.
- A cross-object short asynchronous call contains four padded argument slots.
  The decompiler trims trailing literal zeroes, so a real final argument whose
  value is deliberately zero cannot be distinguished from padding without an
  external signature file.
- The configuration loader keeps one readable name per system-call number even
  though the table contains aliases. A name/signature from a different game
  dialect can therefore be misleading. `WJJR()` demonstrates the safe inline
  assembly fallback for void calls whose encoded signature disagrees.
- Indexed global or configured engine-value increment/decrement currently
  spells the index expression twice when lowering through `Get`/`Set`
  intrinsics. Existing fixtures use side-effect-free indices, but a generated
  temporary is still needed for a side-effecting index.
- Operand-stack joins require identical symbolic expressions. The lifter does
  not yet create a temporary for two different incoming values.
- Natural-loop discovery uses compiler block order and backward edges rather
  than a full dominance analysis. The verified `magic.so` patterns no longer
  require a dispatcher, but unusual graphs may still use that safe fallback.
- The current Sango7 configuration leaves 1,749 of 2,428 functions in naked
  assembly. Those are instruction/system-call dialect gaps, not function-order
  failures.

## 2. Guiding principles

### Correctness before prettiness

The first useful decompiler should emit boring, globally unique labels and
gotos. This already removes most stack-machine syntax and gives editable,
recompilable source. Structured `if`, loop, and switch recovery is a later
transformation of an already correct program.

### Keep lifting and structuralization separate

The old `SSAsm2Pseudo` project had the right architectural instinct: recover
the real expressions and statements independently from the control-flow graph
topology. Its surrogate-x86/Hex-Rays stage should be replaced by a direct
control-flow-graph-to-structure adapter, not reproduced with another native
decompiler.

### Model the virtual machine globally, not as a linear instruction stream

Compiler-generated expressions use postfix, also called reverse Polish,
notation: operands are emitted first and the operator follows. Linear stack
simulation works inside a straight-line block, but fails at joins, loops,
retained switch selectors, spills, and shared tails. The lifter must propagate
symbolic stack states through the control-flow graph until they reach a fixed
point.

### Preserve side effects and evaluation order

Calls, system calls, stores, memory accesses, and configured engine
integer-variable (`INTV`) operations must remain explicit until it is proven
safe to embed them in a larger expression. No C-oriented tool may be allowed
to reorder or duplicate them accidentally.

### Make failure visible and recoverable

Every lift or rewrite should be able to explain why it was rejected: unknown
opcode, inconsistent stack height, conflicting type constraints, unsupported
call graph, unsafe condition rewrite, or failed round-trip verification.

## 3. Existing foundations

The repository already contains most of the format-specific knowledge needed
for a real front end:

- [`cc/Sg2dasm2.cpp`](../cc/Sg2dasm2.cpp) conservatively discovers code from all
  function entries, follows direct control flow, avoids referenced strings,
  and preserves undecoded data.
- [`cc/Sg2dasm2.h`](../cc/Sg2dasm2.h) contains the known opcode and operand-count
  table.
- [`magic_inst.txt`](../magic_inst.txt) documents virtual-machine instruction
  behavior.
- [`fullauto-docs/so-file-format.md`](so-file-format.md) documents
  the object layout and its edge cases.
- [`fullauto-docs/grammar-guide.md`](grammar-guide.md) describes
  the actual source language accepted by the compiler.
- [`cc/TreeAnalysis.cpp`](../cc/TreeAnalysis.cpp) is an executable description of
  how source expressions are lowered to stack instructions.
- [`cc/BranchAnalysis.cpp`](../cc/BranchAnalysis.cpp) shows the exact compiler
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
stack-to-single-definition/data-flow lifting
   |
   +----> explicit side-effect statements
   +----> expression directed acyclic graphs
   +----> type and signature constraints
   |
   v
correct labeled-goto Sango abstract syntax tree  <---- semantic baseline
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

The internal abstract syntax tree must be owned by this project. External
structuralizers may produce or suggest regions, but they must not define the
semantic intermediate representation or source-language rules.

## 5. Core intermediate representations

### Object/module intermediate representation

This layer represents only facts read from the object:

- header fields and preserved raw bytes;
- strings and their original byte encodings;
- function-table records;
- decoded instructions with original addresses and raw operands;
- referenced code/data ranges;
- undecoded or conflicting ranges;
- compiler/assembler trailer data.

It should not yet infer expressions, variables, or structured control flow.

### Function control-flow-graph intermediate representation

A function graph contains:

- basic blocks and original address ranges;
- fallthrough, conditional, switch, call, and return edges;
- a synthetic exit where useful;
- ownership/reachability from each function entry;
- support for shared tails or overlapping entry reachability;
- source-address provenance on every block and instruction.

Function boundaries must not be guessed merely from function-table order,
because the object table does not store function sizes.

### Lifted semantic intermediate representation

Use a static-single-assignment-style value graph with explicit side effects:

- constants and string references;
- local, parameter, global, and configured engine integer-variable (`INTV`)
  reads;
- arithmetic, comparison, conversion, and address operations;
- calls and syscalls with explicit arguments and optional results;
- stores and other state-changing operations;
- terminators (`jump`, conditional jump, switch dispatch, return);
- an ordered symbolic stack at each block entry and exit;
- merged-value block parameters for values joined from different control-flow
  paths.

This need not follow the textbook static single-assignment form used by the
LLVM compiler infrastructure (the project name originally expanded to Low
Level Virtual Machine). The important properties are that each symbolic value
has one definition, joins are explicit, and the operand stack is part of the
data-flow state.

### Structural abstract syntax tree

Keep this deliberately small:

- sequence;
- `if`/`else`;
- loop;
- switch/case;
- break/continue;
- label/goto;
- return;
- optional dispatcher node for an external structurizer's irreducible
  control-flow-graph fallback.

Expressions and side-effect statements should be referenced from the lifted
intermediate representation rather than reconstructed independently by the
structuralizer.

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
2. Return a `Module`/object intermediate representation instead of keeping all
   state inside `main`.
3. Preserve every original byte and address even when a region is not decoded.
4. Keep the existing conservative rule that an unknown opcode stops that
   discovery path.
5. Retain current assembly output as a regression consumer of the new decoder.

Exit criteria:

- current assembly output remains equivalent;
- the same decoded module can be consumed without reparsing text;
- malformed offsets and overlapping code/data produce diagnostics, not memory
  errors.

Compiler concepts exercised: front-end separation and lossless intermediate
representation design.

### Milestone 2: Construct per-function control-flow graphs

Purpose: turn decoded addresses into explicit control flow.

Work:

1. Identify leaders: function entries, branch targets, and instructions after
   terminators/conditional branches.
2. Form maximal basic blocks.
3. Add typed control-flow-graph edges for fallthrough, conditional targets,
   switch cases, jumps, calls, and returns.
4. Compute reachability separately for each function entry.
5. Represent shared tails instead of arbitrarily assigning them to the first
   function visited.
6. Add optional output in Graphviz's DOT graph-description language or
   JavaScript Object Notation for inspection and structuralizer adapters.

Exit criteria:

- every reachable decoded instruction belongs to at least one function graph;
- all direct targets land at block boundaries;
- control-flow-graph visualization agrees with representative disassembly by
  inspection;
- invalid targets remain explicit diagnostics/fallbacks.

Compiler concepts exercised: basic blocks, control-flow graphs, graph
traversal, dominators, and strongly connected components.

### Milestone 3: Lift the operand stack to single-definition values

Purpose: recover expressions and statement boundaries correctly across control
flow.

Work:

1. Define the exact pop/push behavior and side effects of every known opcode.
2. Symbolically execute each block from an incoming ordered stack vector.
3. Propagate outgoing stacks to successors with a worklist algorithm.
4. At joins, require compatible stack heights and create one merged-value
   parameter per differing stack position.
5. Iterate loops until incoming values stabilize.
6. Handle the `JCOND` instruction, which jumps when the retained switch
   selector equals a case value, specially because that selector remains on
   the stack.
7. Obtain direct-call argument counts from the function table and system-call
   argument counts from the packed system-call operand.
8. Keep stores, calls, syscalls, and other effects as ordered statements.
9. Build expression directed acyclic graphs only within the constraints
   imposed by those effects.

If stack heights disagree at a join, do not invent a value. Mark the affected
region/function for assembly fallback and include the predecessor states in the
diagnostic.

Exit criteria:

- straight-line arithmetic, calls, branches, loops, and switches all lift;
- stack height is known at every reachable block boundary;
- expression output is stable regardless of block visitation order;
- the old linear-lifter corpus no longer depends on hand-adjusted input.

Compiler concepts exercised: abstract interpretation, fixed-point data flow,
static single assignment, merged-value nodes, and dominance.

### Milestone 4: Infer types, variables, and signatures

Purpose: emit source that the compiler's type checker accepts without making
unsafe guesses.

Start with a small type lattice:

```text
unknown
  |-- integer/raw 32-bit word/address
  |-- float
  |-- string
  `-- conflict
```

The integer/raw-address classes can initially share source representation
because the compiler intentionally treats integer widths and pointers as a
broad compatibility family. Preserve uncertainty in the intermediate
representation even when the first emitter spells it `int`.

Constraints come from:

- integer, float, and string opcode variants;
- conversions;
- the `PUSHSTR` string-push instruction;
- configured syscall signatures and return categories;
- all direct call sites;
- return instructions and returned values;
- loads/stores of the same virtual-machine slot;
- configured engine integer-variable (`INTV`) and structure information.

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

Purpose: deliver the first true minimum viable decompiler.

Work:

1. Declare parameters and virtual-machine local slots with stable generated
   names.
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
4. Recognize a chain of `JCOND` instructions, which jump when the retained
   selector equals a case value, as a switch. Evaluate the selector once; the
   trailing `POP` only discards that retained value and must not emit another
   call. If case regions have no useful common exit, keep the switch and emit
   localized gotos from its cases instead of expanding the selector tests or
   replacing the whole function with a dispatcher.
5. Convert exits to break/continue where doing so reduces gotos without
   changing their target.
6. Use dominators, postdominators, natural loops, and single-entry/single-exit
   regions for ordinary cases not matched by exact templates.
7. Leave all residual or apparently intentional edges as gotos.

Never combine branch regions into `a && b` or `a || b` merely because a
C-oriented structurer does so. Sango evaluates both logical operands rather
than short-circuiting them. Nested conditions, explicit temporaries, or the
original gotos must preserve the virtual-machine evaluation order.

Exit criteria:

- canonical compiler-generated examples recover their corresponding source
  construct;
- every structured abstract syntax tree can be lowered back to the validated
  goto-based control-flow graph;
- accepting a rewrite cannot reduce verification status;
- residual gotos remain legal and readable.

Compiler concepts exercised: dominators/postdominators, natural loops, region
analysis, and inverse compiler transformations.

### Milestone 7: Evaluate open-source structuralizers

Purpose: decide whether a maintained toolbox materially improves difficult
graphs without becoming the semantic foundation of the project.

Prototype direct adapters for representative control-flow graphs:

#### angr's Structured Angr Intermediate Language Reverter

- Project: <https://github.com/angr/angr>
- Best candidate for decompiler-style readable regions.
- It recursively applies compiler-aware structuring and preserves some gotos
  rather than forcing all graphs into dispatchers.
- Integration requires adapting this project's blocks and conditions to
  angr's intermediate-language and region structures; use it first as a Python
  sidecar experiment.

#### Binaryen Relooper

- Project: <https://github.com/WebAssembly/binaryen>
- C++ implementation designed to turn a general control-flow graph into
  WebAssembly-like block/loop/if/branch shapes.
- Strong candidate for deterministic coverage of arbitrary and irreducible
  graphs.
- Conditions supplied to it must be pure temporary reads because it may
  rearrange or eliminate tests.
- Its irreducible fallback may be a selector-variable dispatcher; retain the
  original goto form whenever that is clearer.

#### Multi-Level Intermediate Representation control flow to structured control flow

- Project: <https://mlir.llvm.org/docs/Passes/#-lift-cf-to-scf>
- Useful modern reference and possible experiment for general
  control-flow-graph-to-structured-control-flow conversion.
- Do not make the LLVM compiler infrastructure (originally named for Low
  Level Virtual Machine) or the Multi-Level Intermediate Representation
  compiler infrastructure a release dependency unless its output proves
  substantially better than the smaller alternatives.

Evaluate each result using:

- recompilation success;
- behavioral/canonical-intermediate-representation verification;
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
    + optional angr Structured Angr Intermediate Language Reverter pass if it
      wins on the real corpus
    + Relooper only when its structured fallback is clearer
```

Compiler concepts exercised: intermediate-representation adapters, region
trees, irreducible flow, and cost models.

### Milestone 8: Strengthen equivalence testing

Purpose: distinguish “it compiled” from “it retained behavior.”

Use several layers of evidence:

1. **Exact binary comparison** for the assembly-preservation mode.
2. **Decoded instruction/action comparison** for code expected to lower into
   the same compiler patterns.
3. **Canonical lifted-intermediate-representation comparison** after
   normalizing labels, addresses, temporary slots, condition inversion, and
   harmless block layout changes.
4. **Differential virtual-machine execution** with identical inputs and
   deterministic system-call stubs, comparing returns, globals, configured
   engine integer-variable (`INTV`) writes, memory writes, and system-call
   traces.
5. **Generated-source testing**: compile small programs exercising every
   language construct and opcode family, decompile them, recompile, and compare.
6. **Fuzz/property testing** for valid stack programs within a constrained
   opcode subset.

An interpreter need not implement the entire game. A useful verification
virtual machine can treat syscalls as typed, trace-producing external
operations and compare the ordered call traces between original and
regenerated functions.

Exit criteria:

- all accepted structural rewrites pass the same checks as goto output;
- verification failures automatically retain or restore the lower-level form;
- corpus reports make regressions visible per function and opcode.

Compiler concepts exercised: interpreters, semantic normalization,
differential testing, and property-based testing.

### Milestone 9: Optional readability improvements

These should not delay the first recompilable version:

- better variable and function names;
- array and structure-member recovery;
- pointer idiom recognition;
- compound assignment and increment recovery;
- `for` preference heuristics;
- common expression and temporary cleanup;
- source comments containing original addresses and uncertainty;
- an interactive control-flow-graph/source view;
- a Ghidra loader and a specification in Ghidra's processor-description
  language, SLEIGH, if interactive reverse engineering becomes a separate
  product goal.

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
- Inline assembly rewriting knows local, parameter, and engine
  integer-variable (`INTV`) names but is not a real assembly parser.
- The function table loses parameter types, return types, local types, local
  names, function sizes, and most high-level aggregate information.
- Code and string data share a section.
- Unknown future opcodes cannot be safely skipped without knowing their operand
  count.
- Configuration files are part of the language's application binary interface
  and must be recorded in a decompilation report.

## 9. Suggested implementation shape

Keep the release core in C++ so it can reuse the current decoder without a
textual or foreign-process boundary. External research adapters may remain
optional Python/tools.

Possible layout:

```text
cc/decompiler/
    InstructionSet.*
    ObjectModel.*
    ObjectDecoder.*
    CodeDiscovery.*
    ConfigurationNames.*
    AssemblyEmitter.*
    ControlFlowGraph.*
    StackLifter.*
    SemanticModel.*
    TypeInference.*
    SangoSyntaxTree.*
    PatternStructurer.*
    SourceNames.*
    SangoEmitter.*
    VerificationReport.*

cc/Sg2dasm2.cpp       assembly command-line interface using the shared decoder
cc/Sg2decomp.cpp      decompiler command-line interface
```

Illustrative command-line interface:

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
- functions with valid control-flow graphs;
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

1. Whether to represent stack joins as conventional merged-value nodes or
   explicit block arguments.
2. Whether shared function tails should be duplicated in source, retained as
   cross-function assembly flow, or assigned to a canonical owner.
3. Whether preserving original function-table order is required by any game
   behavior outside documented callsigns/names.
4. Whether compiler changes such as internal forward declarations are allowed;
   the baseline plan assumes the original `ss.exe` is unchanged.
5. How much canonical intermediate-representation normalization is sufficient
   before a verification virtual machine becomes necessary.
6. Whether angr's Structured Angr Intermediate Language Reverter produces
   enough improvement to justify an optional Python dependency or sidecar
   tool.
7. Whether unknown opcode support should accept a user-supplied operand/stack
   specification.

## 12. Recommended first vertical slice

Do not begin with structuralization. Implement one complete, narrow path:

1. Decode one small `.so` through the shared object model.
2. Build control-flow graphs for all its functions.
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
