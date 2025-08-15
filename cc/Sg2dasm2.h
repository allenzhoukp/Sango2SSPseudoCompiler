#ifndef SG2DASM2_H
#define SG2DASM2_H

#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

#include "ParserObjects.h"

using std::vector;
using std::string;
using std::unordered_map;
using std::map;

const int MAX_LINE_LEN = 1024;
char input[2 << 20];    // 1 MiB

class FixedIdentifierLoader {
public:
    unordered_map<int, Syscall> syscalls;
    unordered_map<int, Intv> intvs;

    void loadSyscallTable ();
    void loadIntvTable ();

} fixedIdentifierLoader;

// a list of all labels (targets for jumps and functions)
// stored in a continuum
struct DAsmLabel {
    int addr;
    string name;
} labels[2 << 18]; 
int labelCount;

struct Line {
    int addr;
    int instCode;
    string content;
    DAsmLabel* label;
    Line() : addr(0), label(nullptr) {};
} ;
// Edit: it is not aligned to 0x4, so no
// Line lines[2 << 18];    // storage for 1 MiB
// instruction lines are aligned at addr / 4 as the .so file is alignd as such
// if addr == 0 then no inst is exactly there

// Use ordered map instead (address -> Line)
std::map<int, Line> lines;

vector<int> stringTable; // index -> addresses.
map<int, string> stringDefs; // address -> content

struct FunctionDef {
    int addr;
    int initStack;
    int description;
    int sequence;
};
vector<FunctionDef> functions;

enum InstType {
    OP_NO, OP_L, OP_LL
};
struct Inst {
    InstType type;
    char symbol[10];
};

Inst instList[0x5A] = {
    {InstType::OP_NO,   "NOP"},       // 0
    {InstType::OP_L,    "RETVAL"},    // 1
    {InstType::OP_L,    "RETN"},      // 2
    {InstType::OP_L,    "STACK"},     // 3
    {InstType::OP_L,    "CALL"},      // 4
    {InstType::OP_LL,   "SYSCALL"},   // 5
    {InstType::OP_L,    "PUSH"},      // 6
    {InstType::OP_L,    "PUSHSTR"},   // 7
    {InstType::OP_L,    "PUSHARG"},   // 8
    {InstType::OP_L,    "PUSHGLB"},     // 9
    {InstType::OP_L,    "PUSHINV"},     // A
    {InstType::OP_L,    "POPN"},        // B
    {InstType::OP_L,    "SETGLB"},      // C
    {InstType::OP_L,    "SETINV"},      // D
    {InstType::OP_NO,   "POP"},         // E
    {InstType::OP_NO,   "ADD"},         // F
    {InstType::OP_NO,   "ADDF"},        // 10
    {InstType::OP_NO,   "ADDS"},        // 11
    {InstType::OP_NO,   "SUB"},         // 12
    {InstType::OP_NO,   "SUBF"},        // 13
    {InstType::OP_NO,   "MUL"},         // 14
    {InstType::OP_NO,   "MULF"},        // 15
    {InstType::OP_NO,   "DIV"},         // 16
    {InstType::OP_NO,   "DIVF"},        // 17
    {InstType::OP_NO,   "MOD"},         // 18
    {InstType::OP_NO,   "NOP19"},       // 19
    {InstType::OP_NO,   "NEG"},         // 1A
    {InstType::OP_NO,   "NEGF"},        // 1B
    {InstType::OP_NO,   "NOT"},         // 1C
    {InstType::OP_L,    "INCN"},        // 1D
    {InstType::OP_L,    "INCGLB"},      // 1E
    {InstType::OP_L,    "INCINV"},      // 1F
    {InstType::OP_L,    "DECN"},        // 20
    {InstType::OP_L,    "DECGLB"},      // 21
    {InstType::OP_L,    "DECINV"},      // 22
    {InstType::OP_NO,   "AND"},         // 23
    {InstType::OP_NO,   "OR"},          // 24
    {InstType::OP_NO,   "XOR"},         // 25
    {InstType::OP_NO,   "CMP"},         // 26
    {InstType::OP_NO,   "CMPF"},        // 27
    {InstType::OP_NO,   "CMPS"},        // 28
    {InstType::OP_NO,   "CMPZ"},        // 29
    {InstType::OP_NO,   "CMPZF"},       // 2A
    {InstType::OP_NO,   "CMPZS"},       // 2B
    {InstType::OP_NO,   "CMPG"},        // 2C
    {InstType::OP_NO,   "CMPGF"},       // 2D
    {InstType::OP_NO,   "CMPGS"},       // 2E
    {InstType::OP_NO,   "CMPGE"},       // 2F
    {InstType::OP_NO,   "CMPGEF"},      // 30
    {InstType::OP_NO,   "CMPGES"},      // 31
    {InstType::OP_NO,   "CMPL"},        // 32
    {InstType::OP_NO,   "CMPLF"},       // 33
    {InstType::OP_NO,   "CMPLS"},       // 34
    {InstType::OP_NO,   "CMPLE"},       // 35
    {InstType::OP_NO,   "CMPLEF"},      // 36
    {InstType::OP_NO,   "CMPLES"},      // 37
    {InstType::OP_NO,   "LNOT"},        // 38
    {InstType::OP_NO,   "LAND"},        // 39
    {InstType::OP_NO,   "LOR"},         // 3A
    {InstType::OP_L,    "JMP"},         // 3B
    {InstType::OP_L,    "JNZ"},         // 3C
    {InstType::OP_L,    "JZ"},          // 3D
    {InstType::OP_NO,   "SHL"},         // 3E
    {InstType::OP_NO,   "SAR"},         // 3F
    {InstType::OP_LL,   "JCOND"},       // 40
    {InstType::OP_NO,   "NOP41"},       // 41
    {InstType::OP_NO,   "DELAY"},       // 42
    {InstType::OP_NO,   "NOP43"},       // 43
    {InstType::OP_NO,   "NOP44"},       // 44
    {InstType::OP_NO,   "WAIT"},        // 45
    {InstType::OP_NO,   "ISRUNNING"},   // 46
    {InstType::OP_NO,   "CALLBS"},      // 47
    {InstType::OP_NO,   "LTOF"},        // 48
    {InstType::OP_NO,   "LTOA"},        // 49
    {InstType::OP_NO,   "FTOL"},        // 4A
    {InstType::OP_NO,   "GCVT"},        // 4B
    {InstType::OP_NO,   "REPZERO"},     // 4C
    {InstType::OP_NO,   "NOP4D"},       // 4D
    {InstType::OP_L,    "PUSHNR"},      // 4E
    {InstType::OP_L,    "PUSHGLBR"},    // 4F
    {InstType::OP_L,    "PUSHINVR"},    // 50
    {InstType::OP_L,    "SETNR"},       // 51
    {InstType::OP_L,    "SETGLBR"},     // 52
    {InstType::OP_L,    "SETINVR"},     // 53
    {InstType::OP_L,    "INCNR"},       // 54
    {InstType::OP_L,    "INCGLBR"},     // 55
    {InstType::OP_L,    "INCINVR"},     // 56
    {InstType::OP_L,    "DECNR"},       // 57
    {InstType::OP_L,    "DECGLBR"},     // 58
    {InstType::OP_L,    "DECINVR"},     // 59
};

#endif