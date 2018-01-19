void function Magic116 () callsign 710 {
    SetInt(0x4A69B0 + 0x10E * 4, 0x46DC60);
    var str = "XIAHOU";
    __asm {
        PUSHINV INTV_ATTACKER_GENERAL
        PUSHARG str
        SYSCALL 0x220, (2 | (0 << 16)) ; SetString
    }
    Prompt(GetINV(INTV_IS_LEFT), GetINV(INTV_ATTACKER_GENERAL), "STOP HIER");
}

#include "magic.cpp"
