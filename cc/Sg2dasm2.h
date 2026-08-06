#ifndef SG2DASM2_H
#define SG2DASM2_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>

#include "ParserObjects.h"

const char* const SYSCALL_TABLE_PATH = "config/SYSCALL_TABLE.txt";
const char* const INTV_TABLE_PATH = "config/INTV_TABLE.txt";

class FixedIdentifierLoader {
public:
    std::unordered_map<uint32_t, Syscall> syscalls;
    std::unordered_map<int32_t, Intv> intvs;

    void loadSyscallTable();
    void loadIntvTable();
};

// Store one decoded instruction and its optional label.
struct Line {
    uint32_t instruction = 0;
    int operandCount = 0;
    int32_t operand = 0;
    int32_t operand2 = 0;
    std::string label;
};

// Store one function-table record.
struct FunctionDef {
    uint32_t addr;
    int32_t initStack;
    uint32_t description;
    int32_t sequence;
};

// Describe one opcode and the number of DWORD operands after it.
struct Inst {
    const char* symbol;
    int operandCount;
};

// Smart Script instructions are DWORDs, followed by zero, one, or two DWORD
// operands. 0x5A and 0x5B are compiler-supported VM extensions emitted through
// assembler macros because sg2asm does not have built-in mnemonics for them.
const Inst instList[] = {
    {"NOP",        0}, // 00
    {"RETVAL",     1}, // 01
    {"RETN",       1}, // 02
    {"STACK",      1}, // 03
    {"CALL",       1}, // 04
    {"SYSCALL",    2}, // 05
    {"PUSH",       1}, // 06
    {"PUSHSTR",    1}, // 07
    {"PUSHARG",    1}, // 08
    {"PUSHGLB",    1}, // 09
    {"PUSHINV",    1}, // 0A
    {"POPN",       1}, // 0B
    {"SETGLB",     1}, // 0C
    {"SETINV",     1}, // 0D
    {"POP",        0}, // 0E
    {"ADD",        0}, // 0F
    {"ADDF",       0}, // 10
    {"ADDS",       0}, // 11
    {"SUB",        0}, // 12
    {"SUBF",       0}, // 13
    {"MUL",        0}, // 14
    {"MULF",       0}, // 15
    {"DIV",        0}, // 16
    {"DIVF",       0}, // 17
    {"MOD",        0}, // 18
    {"NOP19",      0}, // 19
    {"NEG",        0}, // 1A
    {"NEGF",       0}, // 1B
    {"NOT",        0}, // 1C
    {"INCN",       1}, // 1D
    {"INCGLB",     1}, // 1E
    {"INCINV",     1}, // 1F
    {"DECN",       1}, // 20
    {"DECGLB",     1}, // 21
    {"DECINV",     1}, // 22
    {"AND",        0}, // 23
    {"OR",         0}, // 24
    {"XOR",        0}, // 25
    {"CMP",        0}, // 26
    {"CMPF",       0}, // 27
    {"CMPS",       0}, // 28
    {"CMPZ",       0}, // 29
    {"CMPZF",      0}, // 2A
    {"CMPZS",      0}, // 2B
    {"CMPG",       0}, // 2C
    {"CMPGF",      0}, // 2D
    {"CMPGS",      0}, // 2E
    {"CMPGE",      0}, // 2F
    {"CMPGEF",     0}, // 30
    {"CMPGEFS",    0}, // 31, historical assembler spelling for string >=
    {"CMPL",       0}, // 32
    {"CMPLF",      0}, // 33
    {"CMPLS",      0}, // 34
    {"CMPLE",      0}, // 35
    {"CMPLEF",     0}, // 36
    {"CMPLEFS",    0}, // 37, historical assembler spelling for string <=
    {"LNOT",       0}, // 38
    {"LAND",       0}, // 39
    {"LOR",        0}, // 3A
    {"JMP",        1}, // 3B
    {"JNZ",        1}, // 3C
    {"JZ",         1}, // 3D
    {"SHL",        0}, // 3E
    {"SAR",        0}, // 3F
    {"JCOND",      2}, // 40
    {"NOP41",      0}, // 41
    {"DELAY",      0}, // 42
    {"NOP43",      0}, // 43
    {"NOP44",      0}, // 44
    {"WAIT",       0}, // 45
    {"ISRUNNING",  0}, // 46
    {"CALLBS",     0}, // 47
    {"LTOF",       0}, // 48
    {"LTOA",       0}, // 49
    {"FTOL",       0}, // 4A
    {"GCVT",       0}, // 4B
    {"REPLZERO",   0}, // 4C
    {"INST_4D",    0}, // 4D
    {"PUSHNR",     1}, // 4E
    {"PUSHGLBR",   1}, // 4F
    {"PUSHINVR",   1}, // 50
    {"SETNR",      1}, // 51
    {"SETGLBR",    1}, // 52
    {"SETINVR",    1}, // 53
    {"INCNR",      1}, // 54
    {"INCGLBR",    1}, // 55
    {"INCINVR",    1}, // 56
    {"DECNR",      1}, // 57
    {"DECGLBR",    1}, // 58
    {"DECINVR",    1}, // 59
    {"SETUSERVALUE", 1}, // 5A
    {"GETUSERVALUE", 1}, // 5B
};

const std::size_t INST_COUNT = sizeof(instList) / sizeof(instList[0]);

#endif
