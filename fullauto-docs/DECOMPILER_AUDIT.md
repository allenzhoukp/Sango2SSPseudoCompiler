# Decompiler Audit Report

Audit of `cc/decompiler/` (sg2decomp.exe), 2026-08-08.
The decompiler "survived some unit tests but had not been diagnosed carefully"; this report records what the existing tests verify, the defects they missed, and the evidence for each.

## Summary

| # | Defect | Location | Status | Severity |
|---|--------|----------|--------|----------|
| 1 | Float constants decompile as raw integer bits | `cc/decompiler/TypeAnalyzer.cpp` | **Fixed + regression-tested** | High — silent behavior change on recompile |
| 2 | Switch case bodies silently fall through | `cc/decompiler/PatternStructurer.cpp` | **Fixed + regression-tested** | High — wrong codegen; latent (no real object hits it) |
| 3 | `CALL` to a mid-function address emits an undefined assembly label | `cc/decompiler/SangoEmitter.cpp` | **Documented (guarded by test)** | Low — malformed or handcrafted object only |

All existing tests pass before and after the two fixes. Three new focused tests
cover the defects and the documented limitation.

## Method

1. Read every decompiler source file (`cc/decompiler/*.cpp` + `cc/Sg2decomp.cpp`).
2. Built sg2decomp.exe (`make decomp`) and ran the existing suite:
   - `tests/decompiler/roundtrip-windows-interop.sh` — 13 round trips (decompile → compile with ss.exe → assemble → re-decompile), comparing **function interfaces only** (name, return type, parameter count, and call sign).
   - `tests/decompiler/audit-memory-operations.py` — presence/exact counts of memory operations across all fixture objects.
   - `tests/decompiler/compare-structure.py` — structural match of recompiled vs. original decompilation (414/454 functions).
3. Hypothesized defects from reading the code, then demonstrated each on a **synthetic object fixture** built by `tests/decompiler/fixture_objects.py` (hand-encoded Smart Script bytes).
4. Fixed the two semantic defects and added focused regression coverage.
5. Confirmed impact on real objects (`magic.so`, `System.SO`, `SYSTEM_SOMENEW.SO`, `Sango7Script.so`, genskills objects): scan of every emitted region for each defect's shape.

## Defect 1 — Float constants decompile as raw integer bits (FIXED)

**Root cause.** In `TypeAnalyzer::analyze`, the PUSH case (`TypeAnalyzer.cpp:192`) eagerly required `ValueType::integer` on every pushed constant. A later float use (e.g. CMPLF, opcode 0x33) combines that node with `floatingPoint`; `combineTypes` returns `conflict`. `StackLifter` then renders the PUSH's raw bits as an **integer literal** instead of the float.

**Fix.** `PUSH` constants are left untyped so a later use can determine their
type; `PUSHSTR` still requires `string`.

**Evidence.**
- `GetDir` in `magic.so`: before the fix, `if (v2 < 1058261487)`; after, `if (v2 < 0.577300012)` / `else if (v2 >= 1.73199999)`. The integer is the single-precision bit pattern of `0.5773`; ss.exe encodes the decimal literal back to the same bits.
- Bit-exact round trip verified: `0.577300012` as source → ss.exe → `PUSH 1058261487` → decompiler → `0.577300012`.
- The defect is **silent**: recompiling the broken output changes the comparison from `v2 < 0.5773f` to `v2 < 1058261487` (~1.06e9) and compiles cleanly. Program behavior changes without any diagnostic.
- Regression test `tests/decompiler/float-constant-typing.py` FAILED against the reverted binary (`1058261487` present) and PASSES now.

## Defect 2 — Switch case bodies silently fall through (FIXED)

**Root cause.** In `PatternStructurer::emitSwitch` (structured path, join >= 0), a case region whose control flow terminated at a **non-join** boundary (the default target, or a case that is not the next region in emission order) was emitted with **no terminator**. The generated C then fell into the next case body. The `join < 0` (goto-style) path already handled this with an explicit `emitGoto`; the structured path did not.

**Fix.** Emit `break;` only when the region reaches the join. Preserve natural
fallthrough only into the next emitted case or default body. Other edges use
`emitGoto`, which causes `structure` to select the conservative dispatcher.

**Evidence.**
- Synthetic fixture `switch_fallthrough()` (in `tests/decompiler/fixture_objects.py`): `STACK 2; PUSHARG 2; JCOND 1,C1; JCOND 2,C2; POP; JMP D; C1: PUSH 11; POPN 1; JMP D; C2: PUSH 22; POPN 1; JMP J; D: PUSH 33; POPN 1; JMP J; J: PUSHARG 1; RETVAL 1`.
  - Pre-fix output: `case 1:` body has no terminator → a case-1 hit executes case 2's `v1 = 22` **and never reaches `default:`** — three distinct behavior errors (wrong value, missing default side effects, wrong join), and the source compiles cleanly.
  - Post-fix output: dispatcher state 3 assigns `v1 = 11`, then sets `control_state_0 = 5` (default) before state 4 (`v1 = 22`) can run.
- **No real object is affected**: every switch region across `magic.so` (463 functions, 23 switches in its structured decompilation) was scanned; zero have a case body terminating at a non-join boundary. The defect is latent — a real object with this shape would have produced silently wrong code.
- Regression test `tests/decompiler/switch-fallthrough.py` FAILED against the reverted binary (unterminated case 1 body) and PASSES now.

## Defect 3 — `CALL` to a mid-function address emits an undefined label (DOCUMENTED)

**Root cause.** `SangoEmitter::writeAssemblyFunction` collects label targets only from JMP/JZ/JNZ/JCOND operands — **not CALL operands**. A CALL whose operand is a non-entry address therefore references `function_N_block_M` where no such label is defined. sg2asm reports `Error 13 [Symbol not defined.]`.

**Status.** Not fixed. The synthetic fixture calls `0x44` while function entries
are `0x30` and `0x38`. The generated assembly references an undefined internal
label, and `sg2asm.exe` reports `Error 13 [Symbol not defined.]`. The regression
test records this defined failure mode so it cannot be mistaken for a passing
round trip.

## Why the existing tests missed these defects

- `roundtrip-windows-interop.sh` compares **function interfaces only** (signature, parameter count, callsign) — the decompiled *bodies* are never compared, so body corruption (Defect 1, Defect 2) passes silently.
- `audit-memory-operations.py` checks memory operations are represented with exact counts; these defects do not change memory-operation counts.
- `compare-structure.py` compares recompiled output against the *same decompiler's* original output — both sides share the defect, so it self-consistently passes.
- Real-object testing (`magic.so` etc.) missed Defect 1 because the output was only *compiled*, never *re-executed* or compared semantically; and Defect 2 because no real object has the triggering shape.

## Environmental findings (not defects, but they distort testing)

- **sg2asm.exe exits 0 on assembly failure.** Failures are only visible in output text (`Compile failed` / `Error 13 [Symbol not defined.]`). Any script keying on return codes silently reports success.
- **ss.exe resolves output paths against its working directory.** Tests must run it from the repository root; running from another CWD produces "No such file or directory" failures that look like compiler bugs.
- **Windows Subsystem for Linux sandbox flakiness:** blocked writes or Windows interoperability can look like compiler or assembler failures.

## Regression tests added

All under `tests/decompiler/`, driven by the shared fixture builder `fixture_objects.py`:

| Test | Detects | Proof of detection |
|------|---------|--------------------|
| `float-constant-typing.py` | Defect 1 | fails (emits `1058261487`) with fix reverted |
| `switch-fallthrough.py` | Defect 2 | fails (unterminated case body) with fix reverted |
| `mid-function-call.py` | Defect 3 | asserts the defined failure mode persists |

Run the whole suite with `make test-decomp`: 13 interface round trips, the exact
Sango7 assembly path, three focused regression tests, and the memory-operation
audit with exact counts required. The full suite passes with both fixes.

## Differential evidence (carried from prior investigation)

- Recompiling and re-decompiling generated source remains close to direct decompilation of the original object, with known differences from older compiler output. `GetDir` was the clearest float-typing example.
- Regenerated structured output remains close to the manual reference (`REGEN ≈ MANUAL`) for the primary readability target.
- The Genskills objects use conservative dispatchers for their unusual switch exits. Memory-operation counts are exact everywhere.

## Residual risks / recommendations

1. **Semantic, not just syntactic, round-trip checking.** Interface-only round trips will keep missing body corruption. The synthetic-fixture approach used here (encode a known behavior, decompile, assert the behavior textually) is cheap and catches it; expanding it to cover while/continue, switch-join variants, and float-store patterns (STORE + float reuse) is the highest-value next investment.
2. **Defect 2's shape is one compiler version away.** Old-compiler and new-compiler dispatch styles already differ (re-push per JCOND vs. push once); a future compiler change that emits the triggering control flow would turn the latent defect live. The regression test pins it.
3. **Calls to non-entry addresses remain unsupported.** If real handcrafted objects use them, the emitter needs to collect CALL targets as labels and preserve the containing function as assembly.
