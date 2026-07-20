# Compiler smoke-test notes

Last run: 2026-07-19, from `work/v0.9.8-unverified` at `fe24250` plus the
remaining working-tree files.

The current compiler sources were built to `/tmp` so the tracked Windows
`ss.exe` delivery was not overwritten:

```sh
g++ -std=c++11 \
  cc/Main.cpp cc/Lexer.cpp cc/Parser.cpp cc/FixIdentifierLoader.cpp \
  cc/TreeConstruction.cpp cc/TreeAnalysis.cpp cc/BranchAnalysis.cpp \
  cc/Panic.cpp cc/Config.cpp \
  -o /tmp/sango-ss-v098-smoke -g
```

Run the compiler from the repository root and use a bare input filename. The
compiler reads `config/` relative to its current directory and rewrites `/` in
the input argument, so an absolute Unix input path is not safe.

## Results

| Input | Result | Coverage/evidence |
| --- | --- | --- |
| `test.cpp` | pass, 602-line ASM | v0.9.8 globals, global arrays, async calls, float literals, function pointers, and struct-array access. |
| `1.cpp` | pass, 38,467-line ASM | Include expansion and the Big5 string-oriented example. |
| `magic_structured.cpp` | pass, 38,818-line ASM | Large canonical structured magic source. |
| `system_structured.cpp` | pass, 3,242-line ASM | Canonical structured system source. |
| `InterceptorBitset.cpp` | expected incomplete-snippet failure | `Blood` is referenced without an include or declaration; this is not evidence of a parser regression. |
| temporary v0.9.8 extern fixture | pass, 83-line ASM | Global scalar/array writes and a declared external async call; output includes `SETGLBR`, `PUSHGLB`, and `PUSHGLBR`. |

Only compilation to ASM was tested. The `-a` path was not used because it
invokes the Windows assembler and Windows `del`/`copy` commands.

## Unsigned type finding

Bare `unsigned` compiles, but these documented multiword forms currently fail
at the declaration:

- `unsigned int`
- `unsigned short`
- `unsigned byte`

In `Parser::getType`, the `content == "unsigned"` branch is evaluated before
the `combinedContent` branches, so the parser consumes only the first token and
the multiword cases are unreachable. This agrees with the limitation already
recorded in `grammar-guide.md`. Treat this as a known v0.9.8 defect; after it is
fixed, all four spellings should become positive regression cases.
