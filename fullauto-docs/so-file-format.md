# Smart Script `.so` file format

This document describes the little-endian Smart Script object format used by
the `.so` files in this repository. It is based on the compiler/assembler
output and all thirteen available binary fixtures. These files are game script
objects, not ELF shared libraries.

## File layout

All addresses stored in the file are absolute byte offsets from the start of
the file. Instructions, operands, table fields, and table pointers are
32-bit little-endian values.

```text
0x00000000  signature and fixed header
code_start  DWORD bytecode
            NUL-terminated string records (packing varies by producer)
magic_addr  function ("magic") table: 16 bytes per entry
string_addr string pointer table: 4 bytes per entry
file_end    logical end recorded in the header
            optional assembler/compiler provenance trailer
physical EOF
```

In every inspected file, bytecode comes first and the strings form one
contiguous suffix of the code/data section. The format uses pointers rather
than an implicit boundary, so a reader should derive string locations from
both tables instead of guessing that a non-opcode DWORD begins a string.

## Header

The fields through `0x2F` are fixed. The usual code start is `0x40`.

| Offset | Size | Meaning |
| --- | ---: | --- |
| `0x00` | 12 | ASCII signature `Smart Script` (no terminator in these 12 bytes) |
| `0x0C` | 4 | Reserved/padding; zero in all inspected files |
| `0x10` | 4 | Format version; `0x00000100` in all inspected files |
| `0x14` | 4 | Code/data section start; `0x40` in all inspected files |
| `0x18` | 4 | Absolute address of the function table |
| `0x1C` | 4 | Number of function-table entries |
| `0x20` | 4 | Absolute address of the string pointer table |
| `0x24` | 4 | Number of string-table entries |
| `0x28` | 4 | Number of per-script global variables |
| `0x2C` | 4 | Logical file end, normally the end of the string table |
| `0x30` | `code_start - 0x30` | Reserved/fill area |

The reserved area is not a stable magic value. Most fixtures contain four
`0xFFFFFFFF` DWORDs at `0x30`, while the two `froslass_genskills` fixtures and
`Sango7Script.so` contain four zero DWORDs. It must be preserved rather than
normalized.

The inspected files satisfy these equalities:

```text
string_table_addr = magic_table_addr + magic_table_count * 0x10
file_end           = string_table_addr + string_table_count * 0x04
```

They leave no gaps between the two tables or between the string table and the
logical end. A parser can still preserve a gap as raw data if a future file
contains one.

## Code encoding

The VM is stack-based. Each instruction begins with a DWORD opcode and is
followed by zero, one, or two DWORD operands. Consequently, known
instructions occupy 4, 8, or 12 bytes. There is no separate instruction
count or relocation table.

Code discovery starts at every function-table entry and follows direct calls,
jumps, conditional destinations, and ordinary fallthrough. `RETVAL`, `RETN`,
and unconditional `JMP` end a fallthrough path. A referenced string range is
never decoded as an instruction. Bytes not reached by this traversal remain
raw `DD`/`DB` data, which favors exact reconstruction over guessing that an
unreferenced instruction-shaped value is executable code.

Operand counts are:

- Two operands: `0x05` (`SYSCALL`) and `0x40` (`JCOND`).
- One operand: `0x01`-`0x0D` except `0x05`, `0x1D`-`0x22`,
  `0x3B`-`0x3D`, `0x4E`-`0x5B`.
- Zero operands: the remaining defined opcodes from `0x00` through `0x59`.

The `0x4E`-`0x59` instructions are the indirect-addressing variants listed in
`magic_inst.txt`. Opcodes `0x5A` (`SETUSERVALUE`) and `0x5B`
(`GETUSERVALUE`) are extensions emitted by this compiler. The supplied
assembler has no built-in mnemonics for them, so compiler and disassembler
ASM files define macros which emit the opcode and index as two DWORDs.

Two historical assembler spellings remain necessary for reassembly:
`CMPGEFS` for opcode `0x31` (string `>=`) and `CMPLEFS` for opcode `0x37`
(string `<=`). The clearer `CMPGES` and `CMPLES` names documented for v0.9.8
are not accepted by the supplied `sg2asm.exe` binary.

Important operand interpretations:

- `CALL`, `JMP`, `JNZ`, and `JZ` store an absolute code address in their only
  operand.
- `JCOND` stores its comparison value first and its absolute destination
  address second.
- `PUSHSTR` stores a zero-based index into the string pointer table, not a
  direct address.
- `SYSCALL` stores the system-call number first. Its second operand packs the
  parameter count into bits 0-15 and the return/type field into bits 16-31.
- Other operands are immediate signed values or indexes appropriate to their
  instruction. Their binary representation is always the original 32 bits.

The base game instruction set ends at `0x59`; this repository adds `0x5A` and
`0x5B`. A de-assembler cannot infer the operand shape of a later unknown
opcode. The implementation therefore stops that control-flow path and emits
the unclassified bytes as raw data.

A linear interpretation of `Sango7Script.so` contains 222,001 instruction-
shaped records and uses only the same known opcodes from `0x01` through
`0x59`. Conservative control-flow discovery classifies 221,918 of them as
reachable code and leaves 83 records after returns as raw data. Its code still
starts and ends on DWORD boundaries; its string records use arbitrary byte
alignment.

## String storage and string table

A string-table entry is one absolute pointer to string bytes in the code/data
section. A string is stored as its original bytes followed by a NUL. The
supplied files place their strings consecutively at the end of the mixed
code/data section, and their packing depends on the producer:

- The Sango 2/compiler fixtures start each string on a DWORD boundary and use
  zero padding after its NUL.
- `Sango7Script.so` packs all 4,101 function-name and runtime strings directly
  together without padding. Except for the first record, their addresses need
  not be DWORD-aligned.

Adjacency is evidence from the inspected files, not a requirement imposed by
the header. A reader sorts the union of all referenced string addresses and
uses the next address (or the function-table address for the last string) only
as the bound for finding the first NUL. The string ends at that NUL. Zero bytes
through the next absolute DWORD boundary may be retained as conventional
alignment padding; all later bytes remain available for code or raw data.
Consequently, code, referenced strings, and arbitrary gaps can be interleaved
without one string claiming the entire interval before the next string.

Strings are byte sequences, not necessarily UTF-8. This matters for the
project's historical Big5/GB-family input. Reassemblable output should emit
the exact packed bytes, for example:

```asm
STRING_0:
    DD 0x42746547
    DD 0x5F657479
    DD 0x646C4F
    ; "GetByte_Old"
```

The comment is only a readable rendering. The `DD` values are authoritative.
Using packed values also avoids source-encoding conversions by editors or the
assembler.

The string pointer table itself is simply:

```text
string_table_count repetitions of:
    +0x00  uint32 absolute_string_address
```

Repeated pointers are structurally possible even though none of the supplied
fixtures use them. `PUSHSTR n` resolves through entry `n`.

## Function (magic) table

Each function record is 16 bytes:

| Record offset | Size | Meaning |
| --- | ---: | --- |
| `+0x00` | 4 | Absolute bytecode entry address |
| `+0x04` | 4 | Initial stack/argument count |
| `+0x08` | 4 | Absolute pointer to the NUL-terminated function-name string |
| `+0x0C` | 4 | Sequence/callsign value; zero for an ordinary unnumbered function |

The assembler's equivalent is:

```asm
MAGIC_0: procedure GetByte_Old 1 STRING_0 0
```

Function-name pointers and runtime string-table pointers are related but not
identical sets. Compiler-generated fixtures put every function name in the
string table. The original-style `SYSTEM_SOMENEW.SO` has 31 function names
but only two of those addresses also appear among its 43 runtime strings.
`Sango7Script.so` keeps all 2,428 function-name pointers separate from its
1,673 runtime string pointers. Therefore, string discovery must use the union
of:

1. every function record's name pointer, and
2. every string pointer table entry.

Reading only the string table loses original-game function names; scanning
for an invalid opcode can instead mistake arbitrary data or a future opcode
for a string.

## Logical end and physical trailer

The DWORD at header offset `0x2C` points to the end of the VM tables. In the
twelve Sango 2 fixtures it is not physical EOF: the remaining 20, 72, or 116
bytes are NUL-padded provenance strings written by different
assembler/compiler versions, such as:

```text
Script Object Creator: Henryshow
Pseudo Compiler Author: Allen Zhou
```

Newer outputs additionally contain an assembler author, compiler author, and
compilation timestamp. `SYSTEM_SOMENEW.SO` has the shorter
`Creator : HenryShow` trailer. `Sango7Script.so` has no trailer, so its logical
end equals physical EOF.

The header continues to point at the label immediately before this trailer.
The de-assembler emits trailer bytes after `file_end`, preserving both the
logical length and physical file byte-for-byte.

## Fixture evidence

The following header values were checked. “Logical/physical” shows the header
`file_end` followed by the actual byte length.

| File | Function table | Functions | String table | Strings | Logical/physical |
| --- | ---: | ---: | ---: | ---: | ---: |
| `1.so` | `0x3D65C` | 464 | `0x3F35C` | 583 | 261240 / 261312 |
| `magic.so` | `0x3D548` | 463 | `0x3F238` | 580 | 260936 / 261008 |
| `magic_copy.so` | `0x3D618` | 463 | `0x3F308` | 580 | 261144 / 261216 |
| `test.so` | `0xB20` | 11 | `0xBD0` | 13 | 3076 / 3192 |
| `froslass_system.so` | `0x5128` | 40 | `0x53A8` | 82 | 21744 / 21816 |
| `system_dudu_using.so` | `0x881C` | 43 | `0x8ACC` | 87 | 35880 / 35952 |
| `lijing.so` | `0x17BE4` | 180 | `0x18724` | 237 | 101080 / 101152 |
| `SYSTEM_SOMENEW.SO` | `0x4974` | 31 | `0x4B64` | 43 | 19472 / 19492 |
| `froslass_genskills.so` | `0x7BBE0` | 863 | `0x7F1D0` | 988 | 524608 / 524724 |
| `froslass_genskills_gaopei.so` | `0x7C0E4` | 863 | `0x7F6D4` | 988 | 525892 / 526008 |
| `swordheartpuk_genskills.so` | `0x6FDF0` | 798 | `0x72FD0` | 919 | 474668 / 474740 |
| `三个大喝函数求整合.so` | `0x3EFAC` | 475 | `0x40D5C` | 618 | 268036 / 268108 |
| `Sango7Script.so` | `0x1AE8D8` | 2428 | `0x1B8098` | 1673 | 1809084 / 1809084 |

All thirteen de-assemble with the current `cc/Sg2dasm2.cpp`, reassemble with the
supplied `sg2asm.exe`, and compare byte-for-byte with their source `.so` file.

## Assembler compatibility note

Current assembler directives have their conventional widths: `DB`, `DW`, and
`DD` emit 1, 2, and 4 bytes for numeric operands. Oversized values are
silently truncated to the low 8 or 16 bits for `DB` and `DW`; for example,
`DB 0x12345678` emits `0x78` and `DW 0x12345678` emits bytes `78 56`.

A quoted operand includes a trailing NUL and is padded to the directive's
width: `DB "db"` emits `64 62 00`, while `DW "dw"` and `DD "dd"` each emit
their two characters, a NUL, and enough zero bytes to reach a 2- or 4-byte
boundary. Each directive accepts one operand; comma-separated value lists are
rejected as “Too much operand.”

Some older generated `.asm` files in the repository predate the width change
and use `DB 0x12345678` with the old behavior of emitting a DWORD. Such old
ASM is not a reliable syntax reference for the current assembler. The
de-assembler uses `DD` for packed DWORD data and `DB` only for leftover
individual bytes.
