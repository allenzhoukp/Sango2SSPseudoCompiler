#!/usr/bin/env python3
"""Builds minimal Smart Script objects for decompiler regression tests.

Header layout (from fullauto-docs/so-file-format.md and ObjectDecoder.cpp):
  0x00  "Smart Script" signature
  0x10  version
  0x14  codeStart
  0x18  functionTableAddress
  0x1c  functionCount
  0x20  stringTableAddress
  0x24  stringCount
  0x28  globalCount
  0x2c  logicalEnd

Each fixture is a complete, well-formed object.  The first function's code
begins at 0x30, mirroring the compiler's layout.
"""

import struct


def word_offset(instructions, index):
    """Returns the word offset (in 4-byte units) of instruction index."""
    return sum(1 + len(operands) for _, operands in instructions[:index])


def words(instructions):
    """Encodes (opcode, [operands]) pairs as little-endian words."""
    out = []
    for opcode, operands in instructions:
        out.append(struct.pack("<I", opcode))
        for operand in operands:
            out.append(struct.pack("<I", operand & 0xFFFFFFFF))
    return b"".join(out)


def build(name, parameter_count, callsign, code, extra_globals=0,
          runtime_strings=(), function_entries=None, function_names=None):
    """Assembles a complete object with the given code at the first function."""
    code_bytes = words(code)
    code_start = 0x30

    names = list(function_names) if function_names else [name]
    name_blob = b"".join(n.encode("utf-8") + b"\0" for n in names)
    name_addresses = []
    offset = code_start + len(code_bytes)
    for n in names:
        name_addresses.append(offset)
        offset += len(n.encode("utf-8")) + 1
    while offset % 4:
        offset += 1
    function_table = offset
    entries = function_entries if function_entries is not None else [code_start]
    params = [parameter_count] * len(entries)
    signs = [callsign] * len(entries)
    function_table_bytes = b"".join(
        struct.pack("<IIII", entry, p, na, cs)
        for entry, p, na, cs in zip(entries, params, name_addresses, signs))
    string_table = function_table + len(function_table_bytes)
    string_table_bytes = b"".join(struct.pack("<I", a) for a in runtime_strings)
    logical_end = string_table + len(string_table_bytes)

    header = b"Smart Script" + b"\0" * (0x10 - 12)
    header += struct.pack("<IIIIIIII",
                          0x100,            # version
                          code_start,
                          function_table,
                          len(entries),
                          string_table,
                          len(runtime_strings),
                          extra_globals,
                          logical_end)
    padding = b"\0" * ((4 - len(name_blob) % 4) % 4)
    return header + code_bytes + name_blob + padding \
        + function_table_bytes + string_table_bytes


def float_const(name="Test", stack=2, slot=2, bits=0x3F13C9EF, below=1, above=2):
    """Fixture: a float local compared against a raw float constant.

    Source semantics:  int Test() { if (v2 < 0.5773f) return 1; return 2; }
    The comparison must survive recompilation as the same float, not as a
    giant integer literal (the historical mis-typing defect).
    """
    code = [
        (0x03, [stack]),               # STACK
        (0x08, [slot]),                # PUSHARG slot  (float local)
        (0x06, [bits]),                # PUSH raw float bits
        (0x33, []),                    # CMPLF
        (0x3d, [0]),                   # JZ -> ret2 (patched)
        (0x06, [below]),               # PUSH 1
        (0x01, [4]),                   # RETVAL 4
        (0x06, [above]),               # PUSH 2
        (0x01, [4]),                   # RETVAL 4
    ]
    code[4] = (0x3d, [0x30 + 4 * word_offset(code, 7)])
    return build(name, 0, 0, code)


def switch_fallthrough(name="Test"):
    """Fixture: a switch whose first case body jumps directly to default.

    Source semantics:
        switch (v2) {
        case 1: v1 = 11; /* falls into the default body, NOT case 2 */
        case 2: v1 = 22; break;
        default: v1 = 33; break;
        }
        return v1;
    A case-1 hit must end with v1 == 33; case-1 must never execute v1 = 22.
    """
    code = [
        (0x03, [2]), (0x08, [2]), (0x40, [1, 0]), (0x40, [2, 0]),
        (0x0e, []), (0x3b, [0]),
        (0x06, [11]), (0x0b, [1]), (0x3b, [0]),
        (0x06, [22]), (0x0b, [1]), (0x3b, [0]),
        (0x06, [33]), (0x0b, [1]), (0x3b, [0]),
        (0x08, [1]), (0x01, [1]),
    ]
    targets = {
        2: 0x30 + 4 * word_offset(code, 6),   # JCOND 1 -> CASE1
        3: 0x30 + 4 * word_offset(code, 9),   # JCOND 2 -> CASE2
        5: 0x30 + 4 * word_offset(code, 12),  # JMP -> DEFAULT
        8: 0x30 + 4 * word_offset(code, 12),  # CASE1 JMP -> DEFAULT
        11: 0x30 + 4 * word_offset(code, 15), # CASE2 JMP -> JOIN
        14: 0x30 + 4 * word_offset(code, 15), # DEFAULT JMP -> JOIN
    }
    for index, target in targets.items():
        opcode, operands = code[index]
        code[index] = (opcode, operands[:-1] + [target])
    return build(name, 0, 0, code)


def mid_function_call(name="Caller"):
    """Fixture: a CALL whose operand is a mid-function address.

    No well-formed compiler emits this; it exercises the assembly fallback's
    label handling for malformed or hand-written objects.
    """
    code = [
        (0x04, [0x44]), (0x02, [1]),
        (0x06, [1]), (0x01, [1]),
        (0x06, [2]), (0x01, [1]),
    ]
    return build(name, 0, 0, code, function_entries=[0x30, 0x38],
                 function_names=[name, "Callee"])
