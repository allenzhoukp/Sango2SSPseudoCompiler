# Compiler performance investigation

Date: 2026-07-20

## Objective

Identify the causes of slow compilation for long Smart Script inputs and rank
fixes by measured impact, semantic risk, and implementation cost. The initial
prototype changes used for the measurements were made under `/tmp`; the first
optimization batch was implemented after the measurements were reviewed.

## Benchmark method

The baseline compiler was built from the v0.9.8 working sources with GCC 13.3.
Initial prototypes and benchmark outputs were written under `/tmp`.
Correctness comparisons removed only the generated `Compilation Timestamp`
line and then required byte-identical ASM.

Representative inputs:

- `test.cpp`: concentrated v0.9.8 feature smoke test;
- `system_structured.cpp`: medium structured system source;
- `magic_structured.cpp`: 10,912-line, branch-heavy canonical source;
- `1.cpp`: include-based example that expands `magic.cpp`;
- `froslass_genskills.cpp`: large include-based working example producing
  approximately 1.26 MB of ASM.

The normal configuration had redundant-label reduction enabled. The local
`use_exe_memory_io = 0` setting also includes `sg2lang.h`, and was held constant
between compared compiler variants.

## Results

### Primary benchmark: `magic_structured.cpp`

| Compiler variant | Real time | User CPU | Result |
| --- | ---: | ---: | --- |
| Current build, `-O0` | 28.15 s | 27.37 s | Baseline |
| Current code, temporary `-O2` workaround | 3.48 s | 3.73 s | Same normalized ASM |
| Current build, label reduction disabled | 0.86 s | 0.87 s | Diagnostic only; leaves unsafe duplicate labels |
| Two-pass label reducer prototype, `-O0` | 0.87 s | 0.89 s | Same normalized ASM |
| Reducer plus used-node pool reset, `-O0` | 0.78 s | 0.71 s | Same normalized ASM |
| Reducer, pool reset, and struct-name map, `-O0` | 0.61-0.67 s | 0.59-0.60 s | Same normalized ASM |
| All preceding changes, normal `-O2` | 0.36-0.38 s | 0.32-0.33 s | Same normalized ASM |

The tested combination is approximately 74 times faster than the current
unoptimized build on this input.

Other checks:

- `1.cpp`: 1.87 s current versus 0.41 s for the optimized prototype;
- `froslass_genskills.cpp`: 0.77 s for the optimized prototype, producing
  76,217 lines / 1,265,242 bytes of ASM;
- the prototype produced identical normalized output for `test.cpp`,
  `system_structured.cpp`, `magic_structured.cpp`, `1.cpp`, and the O0/O2
  versions of `froslass_genskills.cpp`.

Peak RSS remains approximately 240-257 MB even for small inputs. The executable
has about 148 MB of BSS before the lexer's fixed one-million-token allocation.

## First optimization batch implemented

The first five items in the implementation sequence are now implemented:

- two-pass exact-token reduction of all consecutive assembly labels;
- timed and untimed golden-output fixtures;
- allocation-cursor-only expression-pool clearing;
- normalized structure-name and direct structure-ID lookup;
- a linkable `-O2 -DNDEBUG` release target, with a separate debug build.

Post-implementation validation reproduced the baseline normalized ASM for
`test.cpp`, `system_structured.cpp`, `magic_structured.cpp`, `1.cpp`, and
`froslass_genskills.cpp`. The dedicated label-reduction fixture also reproduced
the legacy reducer's complete normalized output in timed and untimed modes.
Observed post-implementation times were 0.67 seconds at `-O0` and 0.43 seconds
at `-O2` for `magic_structured.cpp`, and 0.75 seconds at `-O2` for
`froslass_genskills.cpp`. Exact times vary between runs; the output checks are
the acceptance criterion.

## Ranked bottlenecks and proposed fixes

### 1. Quadratic redundant-label reduction

`Parser::reduceLabels()` repeatedly regex-scans the entire generated ASM. For
each redundant label it then searches and mutates the entire output string,
before restarting the regex search from the beginning.

On `magic_structured.cpp`, the unreduced output contains 2,963 generated labels:
536 aliases in 414 consecutive-label groups, with groups as long as five. The
existing reducer accounts for roughly 97% of baseline runtime. The larger
Froslass example has 568 aliases in 404 groups, with groups as long as seven.
Therefore, simply disabling reduction is unsafe for the target assembler.

Proposed replacement:

1. Scan emitted ASM lines once and identify runs of consecutive label-only
   declarations, whether compiler-generated or user-defined.
2. Keep the first label in each run and record every later label as an alias of
   the first.
3. Scan output once more, rewriting exact jump/branch label operands through
   the alias map and omitting aliased declarations.
4. Never perform unrestricted substring replacement inside strings or comments.

A temporary two-pass prototype reduced the benchmark from 28.15 s to 0.87 s
at `-O0` and reproduced the old reducer's normalized output exactly.

### 2. Expression pool clears 10,240 nodes per expression

`ExpressionNodePool::clear()` resets every node after every expression,
regardless of how many nodes were used. Profiling with label reduction disabled
showed 8,895 calls on `magic_structured.cpp`, approximately 91 million node
resets, and 47.37% of instrumented self time.

Proposed fix:

- initialize `left`, `right`, and `isLvalue` in `newNode()`;
- make `clear()` reset only the allocation cursor;
- preserve and reuse existing parameter-array storage;
- add an assertion for pool exhaustion.

The temporary implementation preserved output on the large, system, and v0.9.8
smoke inputs. Its incremental impact is much smaller after fixing the label
reducer, so it is the second optimization rather than the first.

### 3. Linear and repeatedly normalized structure lookup

Unknown identifiers are repeatedly tested as possible type names.
`getStructInfoByName()` scans all configured structures and calls `toCamel()` on
both names during each comparison. Profiling observed 3,113,514 `toCamel()`
calls with approximately 85 configured structures.

Proposed fix:

- normalize each structure name once while loading `STRUCT_TABLE.txt`;
- store `normalized name -> StructInfo*` in an `unordered_map`;
- use the sequential structure ID as an indexed lookup after validating bounds.

The prototype reduced the optimized-logic `-O0` benchmark from about 0.78 s to
about 0.61 s while preserving output.

### 4. Optimized release builds currently fail to link

The makefile does not enable optimization. Adding ordinary `-O2` fails because
`Parser::see()` is declared in `Parser.h` but defined as `inline` only in
`Parser.cpp`; other translation units cannot obtain the required definition.

Proposed fix:

- either move the inline definition into `Parser.h`, or remove `inline` from the
  `.cpp` definition;
- enable `-O2` for release builds after output-equivalence testing;
- retain a separate `-O0 -g` developer/debug target.

A temporary `-fkeep-inline-functions` workaround proved that optimization alone
reduces the main benchmark from 28.15 s to 3.48 s. After the algorithmic fixes,
ordinary `-O2` provides a further reduction from roughly 0.63 s to 0.37 s.

### 5. Massive eager fixed-capacity object graphs

The global parser eagerly constructs arrays for 1,024 functions with 1,024
locals each and 1,024 structures with 1,024 members each. Most elements contain
one or more `std::string` objects. The lexer separately constructs 1,048,576
tokens, each containing two strings, regardless of input size.

Proposed fix:

- replace fixed arrays with containers sized by actual use;
- use `reserve()` with realistic starting estimates;
- preserve pointer stability where maps currently retain pointers, using
  `deque`, `unique_ptr`, indexes, or carefully bounded vector reservation;
- represent a token's source file with a file ID rather than copying the file
  name into every token.

This is primarily a memory/startup fix. It should follow the low-risk runtime
changes because it touches object ownership and pointer lifetime.

### 6. Regex-heavy lexing and quadratic text preprocessing

With label reduction disabled, lexer functions account for roughly 31% of the
remaining instrumented runtime. Common whitespace, identifier, numeric,
comment, and operator tokens invoke multiple `std::regex` engines and allocate
temporary match strings.

Macro expansion is more serious asymptotically: every definition scans unread
file buffers, and every occurrence copies the entire remaining suffix through a
fixed 1 MB temporary buffer.

Proposed fix:

- replace common token regexes with direct character scanning and `strto*` or
  `from_chars` conversion;
- expand object-like macros as identifier tokens through a hash map, or build a
  transformed buffer once per pass rather than shifting the suffix per match;
- replace recursive comment skipping in `Lexer::next()` with a loop;
- retain regex temporarily for uncommon macro syntax while migrating.

### 7. Other proportionality problems

- Call nodes allocate/clear 1,025 parameter pointers regardless of actual
  arity; allocate or clear only the requested parameter count.
- Inline-assembly substitution recompiles regexes per local, parameter, and
  INTV and repeatedly shifts the string; scan identifiers once through one
  replacement map.
- Switch parsing allocates 1,024 `ostringstream` objects and 1,024 strings for
  every switch; grow storage by actual case count.
- Several identifier lookups perform a `find()` followed by `operator[]`, or
  copy full `Var` objects repeatedly; return a pointer/reference from one map
  lookup.

## Recommended implementation sequence

1. Replace `reduceLabels()` with the tested two-pass algorithm.
2. Add output-equivalence benchmarks and fixtures for timed and untimed labels.
3. Reset only allocated expression nodes.
4. Add the normalized structure-name map.
5. Repair the `Parser::see()` definition and enable an optimized release target.
6. Replace eager arrays and the fixed token buffer with proportional storage.
7. Optimize common lexer paths and macro/assembly substitution.

The first five items can be delivered in small, independently benchmarked
commits. The container and lexer work should be separate because they carry
more correctness and ownership risk.
