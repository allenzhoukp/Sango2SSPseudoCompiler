#include "sg2lang_v2.h"

void function Magic116 () callsign 710 {
    var r1 = GetShort(0x4A6874) == GetShort_v2(0x4A6874);
    var r2 = GetShort(0x4A6875) == GetShort_v2(0x4A6875);
    var r3 = GetShort(0x4A6876) == GetShort_v2(0x4A6876);
    var r4 = GetShort(0x4A6877) == GetShort_v2(0x4A6877);
    var str;
    __asm {
        PUSHARG r1
        INST_49
        PUSHSTR " "
        ADDS
        PUSHARG r2
        INST_49
        ADDS
        PUSHSTR " "
        ADDS
        PUSHARG r3
        INST_49
        ADDS
        PUSHSTR " "
        ADDS
        PUSHARG r4
        INST_49
        ADDS
        PUSHSTR "%k%%e%"
        ADDS
        POPN str
    }
    Prompt(2, 0, str);
}

#include "magic.cpp"
