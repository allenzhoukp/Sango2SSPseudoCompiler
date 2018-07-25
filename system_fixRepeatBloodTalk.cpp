// --------------------
// This is the adjusted original disassembled code from SYSTEM.SO.
// Most codes remain their assembly form.
// The arguments are assumed to be int, since I don't have time to check the types of all of them.
// Notice: If you are compiling for SYSTEM.SO, theoretically these codes are needed to make it work.
//         However, it is hard not to adjust the original code in practice.
//         Copy and delete some contents in this file if you feel it is needed.
// Notice: This file is encoded with Chinese-Traditional (Big5) charset.
//         Editing (and saving) this file under other charsets may cause messy strings!
//         IF YOU CAN'T SEE THE SENTENCE BELOW, change the charset of your editor to Big5 IMMEDIATELY!
//    Charset example: 如果這行字能正常顯示，那麼您使用的字符集是正確的。
// --------------------

// Added 23 Jul, 2018: A new funtion for promting new general raised by blood.
// ChildGeneralTalk (callsign 10)
// Relative EXE function: SearchGeneralByBlood (0x423476)

// WARNING DO NOT use old exe memory I/O: Game crashed occasionally when calling SearchGeneralByBlood().
// 		Address of PUSHINVR may be modified incorrectly, due to potential mutual exclusion issue.
//      Check config/CONFIG.txt for I/O settings.
// (If you are using an old EXE, rest assured since this method won't be run
//  unless you make a manual call from exe or other functions.)

void naked function MoveCamera (int arg_0, int arg_1, int arg_2) __asm{
LOC_40:
	STACK 5
	SYSCALL 0x111, (0 | (1 << 16)) ; 0x0111
	PUSH 1000
	MUL
	POPN 2
	SYSCALL 0x112, (0 | (1 << 16)) ; 0x0112
	PUSH 1000
	MUL
	POPN 3
	PUSHARG -4
	PUSH 1000
	MUL
	POPN -4
	PUSHARG -3
	PUSH 1000
	MUL
	POPN -3
	PUSHARG -4
	PUSHARG 2
	SUB
	POPN 4
	PUSHARG 4
	PUSHARG -2
	DIV
	POPN 4
	PUSHARG -3
	PUSHARG 3
	SUB
	POPN 5
	PUSHARG 5
	PUSHARG -2
	DIV
	POPN 5
	PUSHARG -2
	POPN 1
LOC_140:
	PUSHARG 2
	PUSHARG 4
	ADD
	POPN 2
	PUSHARG 3
	PUSHARG 5
	ADD
	POPN 3
	PUSHARG 2
	PUSH 1000
	DIV
	PUSHARG 3
	PUSH 1000
	DIV
	SYSCALL 0x100, (2 | (0 << 16)) ; SetViewCamera
	PUSHARG 2
	PUSH 100
	DIV
	PUSHARG -4
	PUSH 100
	DIV
	CMP
	PUSHARG 3
	PUSH 100
	DIV
	PUSHARG -3
	PUSH 100
	DIV
	CMP
	ORNZ
	JZ LOC_220
	PUSH 1
	POPN 1
LOC_220:
	DECN 1
	PUSH 1
	DELAY
	PUSHARG 1
	PUSH 0
	CMPG
	JNZ LOC_140
	RETN 3

}

void naked function CheckGround (int arg_0) __asm{
	RETN 1
LOC_260:
	PUSHARG -2
	SYSCALL 0x107, (1 | (1 << 16)) ; 0x0107
	PUSH 0
	CMPG
	JZ LOC_29C
	PUSH 1
	DELAY
	JMP LOC_260
LOC_29C:
	PUSHARG -2
	PUSH 0
	PUSH 0
	SYSCALL 0x17, (3 | (0 << 16)) ; 0x0017
	PUSHARG -2
	PUSH 4096
	SYSCALL 0x25, (2 | (0 << 16)) ; 0x0025
	PUSHARG -2
	PUSH 16
	PUSH 1
	SYSCALL 0x26, (3 | (0 << 16)) ; 0x0026
	RETN 1

}

void naked function WeatherObjectDeath () callsign 99 __asm{
	STACK 1
	SYSCALL 0xB, (0 | (1 << 16)) ; 0x000B
	POPN 1
	PUSHARG 1
	PUSH 0
	PUSH 0
	SYSCALL 0x17, (3 | (0 << 16)) ; 0x0017
	PUSHARG 1
	PUSH 4096
	SYSCALL 0x25, (2 | (0 << 16)) ; 0x0025
	PUSHARG 1
	PUSH 16
	PUSH 1
	SYSCALL 0x26, (3 | (0 << 16)) ; 0x0026
	RETN 0

}

void naked function ProduceRain () __asm{
	STACK 5
LOC_398:
	PUSH 1
	JZ LOC_50C
	PUSH 3
	PUSH 10
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 5
LOC_3CC:
	PUSHARG 5
	DECN 5
	PUSH 0
	CMPG
	JZ LOC_4F8
	PUSH 0
	PUSH 1024
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 1
	PUSH 0
	PUSH 1024
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 2
	PUSH 300
	PUSH 500
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 3
	PUSHARG 1
	PUSHARG 2
	PUSHARG 3
	PUSH 0
	PUSH 0
	PUSH 2
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	PUSH 9994
	ADD
	SYSCALL 0x10, (5 | (1 << 16)) ; CreateObjectRaw
	POPN 4
	PUSHARG 4
	PUSH 2
	PUSH 5
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	PUSH 0
	SYSCALL 0x17, (3 | (0 << 16)) ; 0x0017
	JMP LOC_3CC
LOC_4F8:
	PUSH 1
	DELAY
	JMP LOC_398
LOC_50C:
	RETN 0

}

void naked function ProduceSnow () __asm{
LOC_514:
	STACK 8
	PUSH 0
	PUSH 255
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 7
	PUSH 1
	PUSH 3
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 6
LOC_564:
	PUSH 1
	JZ LOC_7A8
	PUSH 3
	PUSH 10
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 5
LOC_598:
	PUSHARG 5
	DECN 5
	PUSH 0
	CMPG
	JZ LOC_794
	PUSH 0
	PUSH 1024
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 1
	PUSH 0
	PUSH 1024
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 2
	PUSH 300
	PUSH 500
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 3
	PUSHARG 1
	PUSHARG 2
	PUSHARG 3
	PUSH 0
	PUSH 0
	PUSH 2
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	PUSH 9994
	ADD
	SYSCALL 0x10, (5 | (1 << 16)) ; CreateObjectRaw
	POPN 4
	PUSHARG 4
	PUSHSTR "MAGIC\\M013WingA6.SHP"
	SYSCALL 0x1C, (2 | (0 << 16)) ; 0x001C
	PUSHARG 4
	PUSH 256
	PUSH 512
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0x1D, (2 | (0 << 16)) ; 0x001D
	PUSH 24576
	PUSH 32768
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 8
	PUSHARG 4
	PUSHARG 8
	PUSHARG 8
	SYSCALL 0x1B, (3 | (0 << 16)) ; 0x001B
	PUSH 2
	PUSH 4
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 6
	PUSHARG 4
	PUSHARG 7
	PUSH 10
	NEG
	PUSH 10
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	ADD
	PUSH 0
	PUSHARG 6
	SYSCALL 0x18, (4 | (0 << 16)) ; 0x0018
	JMP LOC_598
LOC_794:
	PUSH 10
	DELAY
	JMP LOC_564
LOC_7A8:
	RETN 0

}

void naked function ProduceFog () __asm{
	STACK 5
	PUSH 0
	PUSH 1024
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 1
	PUSH 0
	PUSH 1024
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 2
	PUSH 300
	PUSH 500
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 3
	PUSHARG 1
	PUSHARG 2
	PUSHARG 3
	PUSH 0
	PUSH 0
	PUSH 2
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	PUSH 9994
	ADD
	SYSCALL 0x10, (5 | (1 << 16)) ; CreateObjectRaw
	POPN 4
	PUSHARG 4
	PUSHSTR "MENU\\GAMEOVER.SHP"
	SYSCALL 0x1C, (2 | (0 << 16)) ; 0x001C
	RETN 0

}

void naked function Battle2 () callsign 62 __asm{
	STACK 7
	RETN 0

}

void naked function EnterWar4 () callsign 64 __asm{
LOC_8B4:
	STACK 5
	PUSH 6
	POPN 1
LOC_8CC:
	PUSHARG 1
	DECN 1
	PUSH 0
	CMPG
	JZ LOC_E0C
	PUSH 14
	PUSH 17
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0x104, (1 | (1 << 16)) ; BattleYToScreenY
	POPN 3
	PUSH 2
	NEG
	PUSH 3
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0x103, (1 | (1 << 16)) ; BattleXToScreenX
	POPN 2
	PUSHARG 2
	PUSH 0
	PUSH 96
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	ADD
	POPN 2
	PUSHARG 3
	PUSH 0
	PUSH 72
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	ADD
	POPN 3
	PUSHARG 2
	PUSHARG 3
	PUSH 0
	PUSH 0
	PUSH 402
	SYSCALL 0x10, (5 | (1 << 16)) ; CreateObjectRaw
	POPN 4
	PUSH 32768
	PUSH 65536
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 5
	PUSHARG 4
	PUSHARG 5
	PUSHARG 5
	SYSCALL 0x1B, (3 | (0 << 16)) ; 0x001B
	PUSH 14
	PUSH 17
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0x104, (1 | (1 << 16)) ; BattleYToScreenY
	POPN 3
	PUSH 4
	PUSH 8
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0x103, (1 | (1 << 16)) ; BattleXToScreenX
	POPN 2
	PUSHARG 2
	PUSH 0
	PUSH 96
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	ADD
	POPN 2
	PUSHARG 3
	PUSH 0
	PUSH 72
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	ADD
	POPN 3
	PUSHARG 2
	PUSHARG 3
	PUSH 0
	PUSH 0
	PUSH 402
	SYSCALL 0x10, (5 | (1 << 16)) ; CreateObjectRaw
	POPN 4
	PUSH 32768
	PUSH 65536
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 5
	PUSHARG 4
	PUSHARG 5
	PUSHARG 5
	SYSCALL 0x1B, (3 | (0 << 16)) ; 0x001B
	PUSH 14
	PUSH 17
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0x104, (1 | (1 << 16)) ; BattleYToScreenY
	POPN 3
	PUSH 48
	PUSH 53
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0x103, (1 | (1 << 16)) ; BattleXToScreenX
	POPN 2
	PUSHARG 2
	PUSH 0
	PUSH 96
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	ADD
	POPN 2
	PUSHARG 3
	PUSH 0
	PUSH 72
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	ADD
	POPN 3
	PUSHARG 2
	PUSHARG 3
	PUSH 0
	PUSH 0
	PUSH 402
	SYSCALL 0x10, (5 | (1 << 16)) ; CreateObjectRaw
	POPN 4
	PUSH 32768
	PUSH 65536
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 5
	PUSHARG 4
	PUSHARG 5
	PUSHARG 5
	SYSCALL 0x1B, (3 | (0 << 16)) ; 0x001B
	PUSH 14
	PUSH 17
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0x104, (1 | (1 << 16)) ; BattleYToScreenY
	POPN 3
	PUSH 54
	PUSH 58
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0x103, (1 | (1 << 16)) ; BattleXToScreenX
	POPN 2
	PUSHARG 2
	PUSH 0
	PUSH 96
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	ADD
	POPN 2
	PUSHARG 3
	PUSH 0
	PUSH 72
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	ADD
	POPN 3
	PUSHARG 2
	PUSHARG 3
	PUSH 0
	PUSH 0
	PUSH 402
	SYSCALL 0x10, (5 | (1 << 16)) ; CreateObjectRaw
	POPN 4
	PUSH 32768
	PUSH 65536
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 5
	PUSHARG 4
	PUSHARG 5
	PUSHARG 5
	SYSCALL 0x1B, (3 | (0 << 16)) ; 0x001B
	JMP LOC_8CC
LOC_E0C:
	PUSH 6
	PUSH 9
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 1
LOC_E30:
	PUSHARG 1
	DECN 1
	PUSH 0
	CMPG
	JZ LOC_FA0
	PUSH 14
	PUSH 17
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0x104, (1 | (1 << 16)) ; BattleYToScreenY
	POPN 3
	PUSH 9
	PUSH 47
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0x103, (1 | (1 << 16)) ; BattleXToScreenX
	POPN 2
	PUSHARG 2
	PUSH 0
	PUSH 96
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	ADD
	POPN 2
	PUSHARG 3
	PUSH 0
	PUSH 72
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	ADD
	POPN 3
	PUSHARG 2
	PUSHARG 3
	PUSH 0
	PUSH 0
	PUSH 402
	SYSCALL 0x10, (5 | (1 << 16)) ; CreateObjectRaw
	POPN 4
	PUSH 32768
	PUSH 65536
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 5
	PUSHARG 4
	PUSHARG 5
	PUSHARG 5
	SYSCALL 0x1B, (3 | (0 << 16)) ; 0x001B
	JMP LOC_E30
LOC_FA0:
	PUSH 6
	POPN 1
LOC_FB0:
	PUSHARG 1
	DECN 1
	PUSH 0
	CMPG
	JZ LOC_126C
	PUSH 14
	PUSH 17
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0x104, (1 | (1 << 16)) ; BattleYToScreenY
	POPN 3
	PUSH 2
	NEG
	PUSH 10
	NEG
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0x103, (1 | (1 << 16)) ; BattleXToScreenX
	POPN 2
	PUSHARG 2
	PUSH 0
	PUSH 96
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	ADD
	POPN 2
	PUSHARG 3
	PUSH 0
	PUSH 72
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	ADD
	POPN 3
	PUSHARG 2
	PUSHARG 3
	PUSH 0
	PUSH 0
	PUSH 402
	SYSCALL 0x10, (5 | (1 << 16)) ; CreateObjectRaw
	POPN 4
	PUSH 32768
	PUSH 65536
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 5
	PUSHARG 4
	PUSHARG 5
	PUSHARG 5
	SYSCALL 0x1B, (3 | (0 << 16)) ; 0x001B
	PUSH 14
	PUSH 17
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0x104, (1 | (1 << 16)) ; BattleYToScreenY
	POPN 3
	PUSH 58
	PUSH 66
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0x103, (1 | (1 << 16)) ; BattleXToScreenX
	POPN 2
	PUSHARG 2
	PUSH 0
	PUSH 96
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	ADD
	POPN 2
	PUSHARG 3
	PUSH 0
	PUSH 72
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	ADD
	POPN 3
	PUSHARG 2
	PUSHARG 3
	PUSH 0
	PUSH 0
	PUSH 402
	SYSCALL 0x10, (5 | (1 << 16)) ; CreateObjectRaw
	POPN 4
	PUSH 32768
	PUSH 65536
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 5
	PUSHARG 4
	PUSHARG 5
	PUSHARG 5
	SYSCALL 0x1B, (3 | (0 << 16)) ; 0x001B
	JMP LOC_FB0
LOC_126C:
	RETN 0

}

void naked function EnterWar5 () callsign 65 __asm{
LOC_1274:
	STACK 5
	PUSH 0
	POPN 1
LOC_128C:
	PUSHARG 1
	PUSH 11
	CMPL
	JZ LOC_1374
	PUSH 15
	SYSCALL 0x104, (1 | (1 << 16)) ; BattleYToScreenY
	POPN 3
	PUSHARG 1
	PUSH 7
	MUL
	SYSCALL 0x103, (1 | (1 << 16)) ; BattleXToScreenX
	POPN 2
	PUSHARG 2
	PUSHARG 3
	PUSH 0
	PUSH 0
	PUSH 403
	SYSCALL 0x10, (5 | (1 << 16)) ; CreateObjectRaw
	POPN 4
	PUSHARG 2
	PUSHARG 3
	PUSH 70
	PUSH 0
	PUSH 404
	SYSCALL 0x10, (5 | (1 << 16)) ; CreateObjectRaw
	POPN 4
	INCN 1
	JMP LOC_128C
LOC_1374:
	RETN 0

}

void naked function EnterWar7 () __asm{
LOC_137C:
	STACK 5
	PUSH 0
	POPN 1
LOC_1394:
	PUSHARG 1
	PUSH 60
	CMPL
	JZ LOC_1518
	PUSH 14
	PUSH 17
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0x104, (1 | (1 << 16)) ; BattleYToScreenY
	POPN 3
	PUSH 0
	PUSH 71
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0x103, (1 | (1 << 16)) ; BattleXToScreenX
	POPN 2
	PUSHARG 2
	PUSH 0
	PUSH 96
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	ADD
	POPN 2
	PUSHARG 3
	PUSH 0
	PUSH 72
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	ADD
	POPN 3
	PUSHARG 2
	PUSHARG 3
	PUSH 0
	PUSH 0
	PUSH 400
	PUSH 401
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0x10, (5 | (1 << 16)) ; CreateObjectRaw
	POPN 4
	PUSH 40960
	PUSH 65536
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 5
	PUSHARG 4
	PUSHARG 5
	PUSHARG 5
	SYSCALL 0x1B, (3 | (0 << 16)) ; 0x001B
	INCN 1
	JMP LOC_1394
LOC_1518:
	RETN 0

}

void naked function LoadingBattleMap (int arg_0) callsign 50 __asm{
	PUSHARG -2
	JCOND 0, LOC_15C4
	JCOND 1, LOC_15CC
	JCOND 2, LOC_15D4
	JCOND 3, LOC_15DC
	JCOND 4, LOC_1600
	JCOND 5, LOC_1630
	JCOND 6, LOC_167C
	JCOND 7, LOC_1684
	JCOND 8, LOC_16B4
	JCOND 9, LOC_1700
	JCOND 10, LOC_1724
	JCOND 11, LOC_172C
	POP
	JMP LOC_1734
LOC_15C4:
	JMP LOC_1734
LOC_15CC:
	JMP LOC_1734
LOC_15D4:
	JMP LOC_1734
LOC_15DC:
	PUSHSTR "BG03A"
	PUSH 255
	SYSCALL 0x301, (2 | (0 << 16)) ; PlaySound1
	JMP LOC_1734
LOC_1600:
	SYSCALL 0x212, (0 | (1 << 16)) ; 0x0212
	PUSH 1
	CMP
	JZ LOC_1628
	CALL EnterWar4
LOC_1628:
	JMP LOC_1734
LOC_1630:
	PUSHSTR "BG04A"
	PUSH 255
	SYSCALL 0x301, (2 | (0 << 16)) ; PlaySound1
	SYSCALL 0x212, (0 | (1 << 16)) ; 0x0212
	PUSH 1
	CMP
	JZ LOC_1674
	CALL EnterWar5
LOC_1674:
	JMP LOC_1734
LOC_167C:
	JMP LOC_1734
LOC_1684:
	SYSCALL 0x212, (0 | (1 << 16)) ; 0x0212
	PUSH 1
	CMP
	JZ LOC_16AC
	CALL EnterWar7
LOC_16AC:
	JMP LOC_1734
LOC_16B4:
	PUSHSTR "BG08A"
	PUSH 255
	SYSCALL 0x301, (2 | (0 << 16)) ; PlaySound1
	SYSCALL 0x212, (0 | (1 << 16)) ; 0x0212
	PUSH 1
	CMP
	JZ LOC_16F8
	CALL ProduceSnow
LOC_16F8:
	JMP LOC_1734
LOC_1700:
	PUSHSTR "BG09A"
	PUSH 255
	SYSCALL 0x301, (2 | (0 << 16)) ; PlaySound1
	JMP LOC_1734
LOC_1724:
	JMP LOC_1734
LOC_172C:
	JMP LOC_1734
LOC_1734:
	RETN 1

}

void naked function BattleYield (int arg_0, int arg_1, int arg_2, int arg_3, int arg_4, int arg_5, int arg_6) __asm{
LOC_173C:
	STACK 4
	PUSHARG -8
	SYSCALL 0x105, (1 | (1 << 16)) ; GetObjectScreenX
	POPN 1
	PUSHARG -8
	SYSCALL 0x106, (1 | (1 << 16)) ; GetObjectScreenY
	POPN 2
	PUSHARG -7
	SYSCALL 0x105, (1 | (1 << 16)) ; GetObjectScreenX
	POPN 3
	PUSHARG -7
	SYSCALL 0x106, (1 | (1 << 16)) ; GetObjectScreenY
	POPN 4
	PUSHARG 1
	PUSHARG 2
	PUSH 128
	SUB
	PUSH 60
	CALL MoveCamera
	PUSHARG -4
	PUSH 0
	PUSHSTR "%S2%將軍，請加入我方，共創統一大業!!%k%%e%"
	SYSCALL 0x3, (3 | (0 << 16)) ; 0x0003
	PUSHARG 3
	PUSHARG 4
	PUSH 128
	SUB
	PUSH 60
	CALL MoveCamera
	PUSHARG -3
	PUSH 0
	PUSHSTR "就讓我們共創統一大業吧!!%k%%e%"
	SYSCALL 0x3, (3 | (0 << 16)) ; 0x0003
	PUSHARG 1
	PUSHARG 2
	PUSH 128
	SUB
	PUSH 60
	CALL MoveCamera
	PUSHARG -4
	PUSH 0
	PUSHSTR "將軍真是明智之舉!!%k%%e%"
	SYSCALL 0x3, (3 | (0 << 16)) ; 0x0003
	PUSHARG -2
	SYSCALL 0x139, (1 | (0 << 16)) ; 0x0139
	PUSH 0
	SYSCALL 0x114, (1 | (0 << 16)) ; 0x0114
	RETN 7

}

void naked function StartBattle (int arg_0) callsign 51 __asm{
	STACK 10
	PUSH 3
	PUSH 1
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	SYSCALL 0x200, (1 | (3 << 16)) ; GetGeneralName
	SYSCALL 0x6, (3 | (0 << 16)) ; 0x0006
	PUSH 2
	PUSH 1
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	SYSCALL 0x200, (1 | (3 << 16)) ; GetGeneralName
	SYSCALL 0x6, (3 | (0 << 16)) ; 0x0006
	PUSH 3
	PUSH 2
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	SYSCALL 0x200, (1 | (3 << 16)) ; GetGeneralName
	SYSCALL 0x6, (3 | (0 << 16)) ; 0x0006
	PUSH 2
	PUSH 2
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	SYSCALL 0x200, (1 | (3 << 16)) ; GetGeneralName
	SYSCALL 0x6, (3 | (0 << 16)) ; 0x0006
	PUSHARG -2
	PUSH 1
	CMP
	JZ LOC_1A08
	PUSHINV 3 ; INTV_DEFENDER_MAJOR
	PUSHINV 2 ; INTV_ATTACKER_MAJOR
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSH 2
	PUSH 3
	PUSH 1
	CALL BattleYield
	RETN 1
	JMP LOC_1A70
LOC_1A08:
	PUSHARG -2
	PUSH 1
	NEG
	CMP
	JZ LOC_1A70
	PUSHINV 2 ; INTV_ATTACKER_MAJOR
	PUSHINV 3 ; INTV_DEFENDER_MAJOR
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	PUSH 3
	PUSH 2
	PUSH 0
	CALL BattleYield
	RETN 1
LOC_1A70:
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	SYSCALL 0x206, (1 | (1 << 16)) ; GetGeneralType
	POPN 6
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	SYSCALL 0x206, (1 | (1 << 16)) ; GetGeneralType
	POPN 5
	PUSHINV 3 ; INTV_DEFENDER_MAJOR
	SYSCALL 0x105, (1 | (1 << 16)) ; GetObjectScreenX
	POPN 8
	PUSHINV 3 ; INTV_DEFENDER_MAJOR
	SYSCALL 0x106, (1 | (1 << 16)) ; GetObjectScreenY
	POPN 9
	PUSHARG 8
	PUSHARG 9
	PUSH 128
	SUB
	PUSH 30
	CALL MoveCamera
	PUSHARG 5
	JCOND 0, LOC_1B44
	JCOND 1, LOC_1BCC
	JCOND 2, LOC_1C54
	POP
	JMP LOC_1DD8
LOC_1B44:
	PUSH 0
	PUSH 1
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	PUSH 0
	CMP
	JZ LOC_1BA0
	PUSH 300
	PUSH 319
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 3
	JMP LOC_1BC4
LOC_1BA0:
	PUSH 380
	PUSH 399
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 3
LOC_1BC4:
	JMP LOC_1DD8
LOC_1BCC:
	PUSH 0
	PUSH 1
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	PUSH 0
	CMP
	JZ LOC_1C28
	PUSH 320
	PUSH 339
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 3
	JMP LOC_1C4C
LOC_1C28:
	PUSH 380
	PUSH 399
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 3
LOC_1C4C:
	JMP LOC_1DD8
LOC_1C54:
	PUSHARG 6
	PUSH 0
	CMP
	JZ LOC_1D50
	PUSH 0
	PUSH 2
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 7
	PUSHARG 7
	PUSH 0
	CMP
	JZ LOC_1CDC
	PUSH 340
	PUSH 359
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 3
	JMP LOC_1D48
LOC_1CDC:
	PUSHARG 7
	PUSH 1
	CMP
	JZ LOC_1D24
	PUSH 380
	PUSH 399
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 3
	JMP LOC_1D48
LOC_1D24:
	PUSH 360
	PUSH 379
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 3
LOC_1D48:
	JMP LOC_1DD0
LOC_1D50:
	PUSH 0
	PUSH 1
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	PUSH 0
	CMP
	JZ LOC_1DAC
	PUSH 340
	PUSH 359
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 3
	JMP LOC_1DD0
LOC_1DAC:
	PUSH 380
	PUSH 399
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 3
LOC_1DD0:
	JMP LOC_1DD8
LOC_1DD8:
	PUSH 2
	PUSH 0
	PUSHARG 3
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSHINV 2 ; INTV_ATTACKER_MAJOR
	SYSCALL 0x105, (1 | (1 << 16)) ; GetObjectScreenX
	POPN 8
	PUSHINV 2 ; INTV_ATTACKER_MAJOR
	SYSCALL 0x106, (1 | (1 << 16)) ; GetObjectScreenY
	POPN 9
	PUSHARG 8
	PUSHARG 9
	PUSH 128
	SUB
	PUSH 60
	CALL MoveCamera
LOC_1E60:
	PUSHARG 6
	JCOND 0, LOC_1E98
	JCOND 1, LOC_1F78
	JCOND 2, LOC_2058
	POP
	JMP LOC_227C
LOC_1E98:
	PUSH 0
	PUSH 2
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 7
	PUSHARG 7
	PUSH 0
	CMP
	JZ LOC_1F04
	PUSH 300
	PUSH 319
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 4
	JMP LOC_1F70
LOC_1F04:
	PUSHARG 7
	PUSH 1
	CMP
	JZ LOC_1F4C
	PUSH 380
	PUSH 399
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 4
	JMP LOC_1F70
LOC_1F4C:
	PUSH 400
	PUSH 409
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 4
LOC_1F70:
	JMP LOC_227C
LOC_1F78:
	PUSH 0
	PUSH 2
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 7
	PUSHARG 7
	PUSH 0
	CMP
	JZ LOC_1FE4
	PUSH 320
	PUSH 339
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 4
	JMP LOC_2050
LOC_1FE4:
	PUSHARG 7
	PUSH 1
	CMP
	JZ LOC_202C
	PUSH 380
	PUSH 399
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 4
	JMP LOC_2050
LOC_202C:
	PUSH 400
	PUSH 409
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 4
LOC_2050:
	JMP LOC_227C
LOC_2058:
	PUSHARG 5
	PUSH 0
	CMP
	JZ LOC_219C
	PUSH 0
	PUSH 3
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 7
	PUSHARG 7
	PUSH 0
	CMP
	JZ LOC_20E0
	PUSH 340
	PUSH 359
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 4
	JMP LOC_2194
LOC_20E0:
	PUSHARG 7
	PUSH 1
	CMP
	JZ LOC_2128
	PUSH 380
	PUSH 399
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 4
	JMP LOC_2194
LOC_2128:
	PUSHARG 7
	PUSH 2
	CMP
	JZ LOC_2170
	PUSH 360
	PUSH 379
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 4
	JMP LOC_2194
LOC_2170:
	PUSH 400
	PUSH 409
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 4
LOC_2194:
	JMP LOC_2274
LOC_219C:
	PUSH 0
	PUSH 2
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 7
	PUSHARG 7
	PUSH 0
	CMP
	JZ LOC_2208
	PUSH 340
	PUSH 359
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 4
	JMP LOC_2274
LOC_2208:
	PUSHARG 7
	PUSH 1
	CMP
	JZ LOC_2250
	PUSH 380
	PUSH 399
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 4
	JMP LOC_2274
LOC_2250:
	PUSH 400
	PUSH 409
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 4
LOC_2274:
	JMP LOC_227C
LOC_227C:
	PUSHARG 4
	PUSHARG 3
	CMP
	JNZ LOC_1E60
	PUSH 3
	PUSH 0
	PUSHARG 4
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSH 1
	SYSCALL 0x114, (1 | (0 << 16)) ; 0x0114
	RETN 1

}

void naked function AllRush () callsign 59 __asm{
	STACK 3
	SYSCALL 0x131, (0 | (0 << 16)) ; DisablePlayMagic
	SYSCALL 0x122, (0 | (0 << 16)) ; SetAttackCounterTo9999 or DisableAttack
	PUSHSTR "MoveCamera"
	INST_45
	PUSHINV 2 ; INTV_ATTACKER_MAJOR
	SYSCALL 0x105, (1 | (1 << 16)) ; GetObjectScreenX
	POPN 1
	PUSHINV 2 ; INTV_ATTACKER_MAJOR
	SYSCALL 0x106, (1 | (1 << 16)) ; GetObjectScreenY
	POPN 2
	PUSHINV 2 ; INTV_ATTACKER_MAJOR
	PUSH 65536
	SYSCALL 0x120, (2 | (0 << 16)) ; SetObjectAnimate
	PUSHARG 1
	PUSHARG 2
	PUSH 128
	SUB
	PUSH 20
	CALL MoveCamera
	SYSCALL 0x123, (0 | (0 << 16)) ; RestoreAttackCounter
	PUSH 155
	PUSHINV 5 ; INTV_IS_RIGHT
	SYSCALL 0x10C, (1 | (1 << 16)) ; GetSoldierCount
	PUSH 2
	DIV
	ADD
	POPN 3
	PUSHARG 3
	PUSH 255
	CMPG
	JZ LOC_23F0
	PUSH 255
	POPN 3
LOC_23F0:
	PUSHSTR "Rush01"
	PUSHARG 3
	SYSCALL 0x301, (2 | (0 << 16)) ; PlaySound1
	PUSH 3
	PUSHINV 5 ; INTV_IS_RIGHT
	SUB
	PUSH 0
	PUSHSTR "全軍突擊!%k%%e%"
	SYSCALL 0x3, (3 | (0 << 16)) ; 0x0003
	PUSH 44
	DELAY
	SYSCALL 0x110, (0 | (0 << 16)) ; EnablePlayMagic
	RETN 0

}

void naked function BattleEndTalk (int arg_0) callsign 56 __asm{
	STACK 7
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	SYSCALL 0x206, (1 | (1 << 16)) ; GetGeneralType
	POPN 4
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	SYSCALL 0x206, (1 | (1 << 16)) ; GetGeneralType
	POPN 3
	PUSHARG -2
	JCOND 1, LOC_2504
	JCOND 2, LOC_25A8
	JCOND 256, LOC_264C
	JCOND 512, LOC_2668
	JCOND 4, LOC_2684
	JCOND 8, LOC_26E4
	JCOND 16, LOC_2744
	POP
	JMP LOC_27A4
LOC_2504:
	PUSH 2
	POPN 1
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	SYSCALL 0x200, (1 | (3 << 16)) ; GetGeneralName
	POPN 6
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	SYSCALL 0x200, (1 | (3 << 16)) ; GetGeneralName
	POPN 7
	PUSH 1
	POPN 2
	PUSHARG 3
	PUSH 2
	CMP
	PUSHARG 4
	PUSH 0
	CMP
	ORNZ
	JZ LOC_25A0
	PUSH 3
	POPN 2
LOC_25A0:
	JMP LOC_27A4
LOC_25A8:
	PUSH 3
	POPN 1
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	SYSCALL 0x200, (1 | (3 << 16)) ; GetGeneralName
	POPN 7
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	SYSCALL 0x200, (1 | (3 << 16)) ; GetGeneralName
	POPN 6
	PUSH 1
	POPN 2
	PUSHARG 4
	PUSH 2
	CMP
	PUSHARG 3
	PUSH 0
	CMP
	ORNZ
	JZ LOC_2644
	PUSH 3
	POPN 2
LOC_2644:
	JMP LOC_27A4
LOC_264C:
	PUSH 1
	NEG
	POPN 2
	JMP LOC_27A4
LOC_2668:
	PUSH 1
	NEG
	POPN 2
	JMP LOC_27A4
LOC_2684:
	PUSH 2
	POPN 1
	PUSH 2
	POPN 2
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	SYSCALL 0x200, (1 | (3 << 16)) ; GetGeneralName
	POPN 6
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	SYSCALL 0x200, (1 | (3 << 16)) ; GetGeneralName
	POPN 7
	JMP LOC_27A4
LOC_26E4:
	PUSH 3
	POPN 1
	PUSH 2
	POPN 2
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	SYSCALL 0x200, (1 | (3 << 16)) ; GetGeneralName
	POPN 7
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	SYSCALL 0x200, (1 | (3 << 16)) ; GetGeneralName
	POPN 6
	JMP LOC_27A4
LOC_2744:
	PUSH 3
	POPN 1
	PUSH 0
	POPN 2
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	SYSCALL 0x200, (1 | (3 << 16)) ; GetGeneralName
	POPN 7
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	SYSCALL 0x200, (1 | (3 << 16)) ; GetGeneralName
	POPN 6
	JMP LOC_27A4
LOC_27A4:
	PUSHARG 1
	PUSH 1
	PUSHARG 7
	SYSCALL 0x6, (3 | (0 << 16)) ; 0x0006
	PUSHARG 1
	PUSH 2
	PUSHARG 6
	SYSCALL 0x6, (3 | (0 << 16)) ; 0x0006
	PUSHARG 2
	JCOND 0, LOC_2830
	JCOND 1, LOC_2870
	JCOND 2, LOC_28B0
	JCOND 3, LOC_28F0
	POP
	JMP LOC_2930
LOC_2830:
	PUSHARG 1
	PUSH 0
	PUSH 270
	PUSH 290
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	JMP LOC_2930
LOC_2870:
	PUSHARG 1
	PUSH 0
	PUSH 500
	PUSH 568
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	JMP LOC_2930
LOC_28B0:
	PUSHARG 1
	PUSH 0
	PUSH 600
	PUSH 705
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	JMP LOC_2930
LOC_28F0:
	PUSHARG 1
	PUSH 0
	PUSH 240
	PUSH 263
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	JMP LOC_2930
LOC_2930:
	SYSCALL 0x124, (0 | (0 << 16)) ; 0x0124
	RETN 1

}

void naked function ReportResult (int arg_0) callsign 6 __asm{
	PUSH 1
	DELAY
	PUSH 0
	PUSH 1
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	SYSCALL 0x200, (1 | (3 << 16)) ; GetGeneralName
	SYSCALL 0x6, (3 | (0 << 16)) ; 0x0006
	PUSH 0
	PUSH 2
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	SYSCALL 0x200, (1 | (3 << 16)) ; GetGeneralName
	SYSCALL 0x6, (3 | (0 << 16)) ; 0x0006
	PUSH 0
	PUSH 3
	PUSHINV 9 ; INTV_ATTACKER_KING_GENERAL
	SYSCALL 0x200, (1 | (3 << 16)) ; GetGeneralName
	SYSCALL 0x6, (3 | (0 << 16)) ; 0x0006
	PUSHARG -2
	JCOND 1, LOC_2A48
	JCOND 2, LOC_2A74
	JCOND 4, LOC_2AA0
	JCOND 8, LOC_2ACC
	JCOND 256, LOC_2AF8
	JCOND 512, LOC_2B24
	JCOND 16, LOC_2B50
	POP
	JMP LOC_2B7C
LOC_2A48:
	PUSH 0
	PUSHINV 9 ; INTV_ATTACKER_KING_GENERAL
	PUSHSTR "很遺憾，我軍戰敗了....%K%%E%"
	SYSCALL 0x3, (3 | (0 << 16)) ; 0x0003
	JMP LOC_2BA8
LOC_2A74:
	PUSH 0
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSHSTR "我軍勝利!!%K%%E%"
	SYSCALL 0x3, (3 | (0 << 16)) ; 0x0003
	JMP LOC_2BA8
LOC_2AA0:
	PUSH 0
	PUSHINV 9 ; INTV_ATTACKER_KING_GENERAL
	PUSHSTR "我軍戰敗，%S1%將軍被俘了....%K%%E%"
	SYSCALL 0x3, (3 | (0 << 16)) ; 0x0003
	JMP LOC_2BA8
LOC_2ACC:
	PUSH 0
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSHSTR "我軍大勝!俘虜敵將%S2%!!%K%%E%"
	SYSCALL 0x3, (3 | (0 << 16)) ; 0x0003
	JMP LOC_2BA8
LOC_2AF8:
	PUSH 0
	PUSHINV 9 ; INTV_ATTACKER_KING_GENERAL
	PUSHSTR "%S1%將軍投靠敵方了....%K%%E%"
	SYSCALL 0x3, (3 | (0 << 16)) ; 0x0003
	JMP LOC_2BA8
LOC_2B24:
	PUSH 0
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSHSTR "說服%S2%將軍加入我方!!%K%%E%"
	SYSCALL 0x3, (3 | (0 << 16)) ; 0x0003
	JMP LOC_2BA8
LOC_2B50:
	PUSH 0
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSHSTR "雙方平手....%K%%E%"
	SYSCALL 0x3, (3 | (0 << 16)) ; 0x0003
	JMP LOC_2BA8
LOC_2B7C:
	PUSH 0
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSHSTR "結果不明.......%K%%E%"
	SYSCALL 0x3, (3 | (0 << 16)) ; 0x0003
	JMP LOC_2BA8
LOC_2BA8:
	RETN 1

}

void naked function SearchTalk (int arg_0) callsign 5 __asm{
LOC_2BB0:
	INST_09 1
	PUSH 0
	CMPZ
	JZ LOC_2BE0
	PUSH 1
	DELAY
	JMP LOC_2BB0
LOC_2BE0:
	PUSH 1
	SETARG 1
	PUSH 0
	PUSH 1
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	SYSCALL 0x200, (1 | (3 << 16)) ; GetGeneralName
	SYSCALL 0x6, (3 | (0 << 16)) ; 0x0006
	PUSH 0
	PUSH 3
	PUSHINV 9 ; INTV_ATTACKER_KING_GENERAL
	SYSCALL 0x200, (1 | (3 << 16)) ; GetGeneralName
	SYSCALL 0x6, (3 | (0 << 16)) ; 0x0006
	PUSHARG -2
	JCOND 0, LOC_2CB8
	JCOND 1, LOC_2CE4
	JCOND 16, LOC_2D10
	JCOND 17, LOC_2EA8
	JCOND 18, LOC_3054
	JCOND 19, LOC_3170
	JCOND 20, LOC_328C
	POP
	JMP LOC_34F4
LOC_2CB8:
	PUSH 0
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSHSTR "什麼都找不到....%k%%E%"
	SYSCALL 0x3, (3 | (0 << 16)) ; 0x0003
	JMP LOC_34F4
LOC_2CE4:
	PUSH 0
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSHSTR "找到物品!!%k%%E%"
	SYSCALL 0x3, (3 | (0 << 16)) ; 0x0003
	JMP LOC_34F4
LOC_2D10:
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	CMPZ
	JZ LOC_2D50
	PUSH 0
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSH 800
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
LOC_2D50:
	PUSH 0
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	PUSH 801
	PUSH 809
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSH 1
	PUSHINV 9 ; INTV_ATTACKER_KING_GENERAL
	PUSH 810
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSHSTR "SUCCESS"
	PUSH 255
	SYSCALL 0x301, (2 | (0 << 16)) ; PlaySound1
	PUSH 0
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	PUSH 819
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSH 1
	PUSHINV 9 ; INTV_ATTACKER_KING_GENERAL
	PUSHSTR "%e%"
	SYSCALL 0x3, (3 | (0 << 16)) ; 0x0003
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	CMPZ
	JZ LOC_2E7C
	PUSH 0
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	PUSH 812
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSH 0
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSH 813
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	JMP LOC_2EA0
LOC_2E7C:
	PUSH 0
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	PUSHSTR "%e%"
	SYSCALL 0x3, (3 | (0 << 16)) ; 0x0003
LOC_2EA0:
	JMP LOC_34F4
LOC_2EA8:
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	CMPZ
	JZ LOC_2EE8
	PUSH 0
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSH 800
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
LOC_2EE8:
	PUSH 1
	PUSHINV 9 ; INTV_ATTACKER_KING_GENERAL
	PUSH 820
	PUSH 826
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSH 0
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	PUSH 830
	PUSH 840
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSHSTR "SUCCESS"
	PUSH 255
	SYSCALL 0x301, (2 | (0 << 16)) ; PlaySound1
	PUSH 1
	PUSHINV 9 ; INTV_ATTACKER_KING_GENERAL
	PUSH 811
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSH 1
	PUSHINV 9 ; INTV_ATTACKER_KING_GENERAL
	PUSHSTR "%e%"
	SYSCALL 0x3, (3 | (0 << 16)) ; 0x0003
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	CMPZ
	JZ LOC_3028
	PUSH 0
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	PUSH 812
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSH 0
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSH 813
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	JMP LOC_304C
LOC_3028:
	PUSH 0
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	PUSHSTR "%e%"
	SYSCALL 0x3, (3 | (0 << 16)) ; 0x0003
LOC_304C:
	JMP LOC_34F4
LOC_3054:
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	CMPZ
	JZ LOC_3094
	PUSH 0
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSH 800
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
LOC_3094:
	PUSH 0
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	PUSH 801
	PUSH 809
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSH 1
	PUSHINV 9 ; INTV_ATTACKER_KING_GENERAL
	PUSH 851
	PUSH 861
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSHSTR "FAILURE2"
	PUSH 255
	SYSCALL 0x301, (2 | (0 << 16)) ; PlaySound1
	PUSH 0
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	PUSH 814
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSH 1
	PUSHINV 9 ; INTV_ATTACKER_KING_GENERAL
	PUSHSTR "%e%"
	SYSCALL 0x3, (3 | (0 << 16)) ; 0x0003
	JMP LOC_34F4
LOC_3170:
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	CMPZ
	JZ LOC_31B0
	PUSH 0
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSH 800
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
LOC_31B0:
	PUSH 0
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	PUSH 801
	PUSH 809
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSH 1
	PUSHINV 9 ; INTV_ATTACKER_KING_GENERAL
	PUSH 850
	PUSH 860
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSHSTR "FAILURE2"
	PUSH 255
	SYSCALL 0x301, (2 | (0 << 16)) ; PlaySound1
	PUSH 0
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	PUSH 815
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSH 1
	PUSHINV 9 ; INTV_ATTACKER_KING_GENERAL
	PUSHSTR "%e%"
	SYSCALL 0x3, (3 | (0 << 16)) ; 0x0003
	JMP LOC_34F4
LOC_328C:
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	CMP
	JZ LOC_33BC
	PUSH 0
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSH 816
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSH 1
	PUSHINV 9 ; INTV_ATTACKER_KING_GENERAL
	PUSH 870
	PUSH 874
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSHSTR "SUCCESS"
	PUSH 255
	SYSCALL 0x301, (2 | (0 << 16)) ; PlaySound1
	PUSH 0
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	PUSH 875
	PUSH 878
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSH 1
	PUSHINV 9 ; INTV_ATTACKER_KING_GENERAL
	PUSH 890
	PUSH 893
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSH 0
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	PUSHSTR "%e%"
	SYSCALL 0x3, (3 | (0 << 16)) ; 0x0003
	JMP LOC_34EC
LOC_33BC:
	PUSH 0
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSH 817
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSH 1
	PUSHINV 9 ; INTV_ATTACKER_KING_GENERAL
	PUSH 818
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSHSTR "SUCCESS"
	PUSH 255
	SYSCALL 0x301, (2 | (0 << 16)) ; PlaySound1
	PUSH 0
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	PUSH 894
	PUSH 899
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSH 1
	PUSHINV 9 ; INTV_ATTACKER_KING_GENERAL
	PUSH 900
	PUSH 903
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSH 0
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	PUSH 812
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSH 0
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSH 904
	PUSH 907
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
LOC_34EC:
	JMP LOC_34F4
LOC_34F4:
	PUSH 0
	SETARG 1
	RETN 1

}

// Added 23 Jul, 2018: A new funtion for promting new general raised by blood.
// ChildGeneralTalk (callsign 10)
// Relative EXE function: SearchGeneralByBlood (0x423476)

// Main execution function
void function PromptChildGenTalk (General* pChild) {
	do {
		Delay(1);
	} while(GetGlobal(1) != 0);
	SetGlobal(1, 1);

	General* pParent = pChild->pBloodGeneral;
	General* pKingGeneral = pParent->pKing->pGeneral;

	SetSXString(0, 3, GetGeneralName(pChild));	// %S3%
	SetSXString(0, 1, GetGeneralName(pParent));	// %S1%

	// King's child
	if (pParent == pKingGeneral) {
		PromptWithMessageIndex1(0, pParent, 816);	// "%S3%已經成年了....%k%%l%"
		PromptWithMessageIndex1(1, pChild, Rand(870, 874));	// e.g. "父親大人，我已經準備好了，請讓我盡一份力吧！%k%%l%"
		PlaySound1("SUCCESS", 255);

		PromptWithMessageIndex1(0, pKingGeneral, Rand(875, 878)); // e.g. "你能加入我就更放心了！%k%%l%"
		PromptWithMessageIndex1(1, pChild, Rand(890, 893)); // e.g. "父親大人，我一定不會辜負您的期待的！%K%%e%"
		Prompt1(0, pKingGeneral, "%e");

	// Suggested by parent
	} else {
		PromptWithMessageIndex1(0, pParent, 817);	// "大人，%S3%已成年了，請大人會見他。%k%%l%"
		PromptWithMessageIndex1(1, pChild, 818);   // "請讓我為大人效力吧！！%k%%l%"
		PlaySound1("SUCCESS", 255);

		PromptWithMessageIndex1(0, pKingGeneral, Rand(894, 899));   // e.g. "好好表現！不要讓你父親失望了！%k%%l%"
		PromptWithMessageIndex1(1, pChild, Rand(900, 903));			// e.g. "太好了，我等這一天已經很久了！%K%%e%"
		PromptWithMessageIndex1(0, pKingGeneral, 812);	// "%S1%將軍，辛苦了！%K%%l%"
		PromptWithMessageIndex1(0, pParent, Rand(904, 907));		// e.g. "這是應該的，希望他能不負大人所望！%k%%e%"
	}


	Delay(1);

	SetGlobal(1, 0);
}

// Facade method.
// This indirect asynccall is for mutual exclusion.
void function ChildGeneralTalk (General* pChild) callsign 10 {
	Wait("PromptChildGenTalk");
	asynccall PromptChildGenTalk(pChild);
}

void naked function KillGeneral () callsign 70 __asm{
	STACK 1
	PUSH 0
	PUSH 10
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	POPN 1
	PUSH 0
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSH 910
	PUSHARG 1
	PUSH 2
	MUL
	ADD
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSH 1
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	PUSH 910
	PUSHARG 1
	PUSH 2
	MUL
	ADD
	PUSH 1
	ADD
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSH 0
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSHSTR "%E%"
	SYSCALL 0x3, (3 | (0 << 16)) ; 0x0003
	PUSH 20
	DELAY
	SYSCALL 0x209, (0 | (0 << 16)) ; 0x0209
	RETN 0

}

void naked function FreeGeneral () callsign 71 __asm{
	PUSH 0
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSH 940
	PUSH 945
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSH 20
	DELAY
	SYSCALL 0x210, (0 | (0 << 16)) ; 0x0210
	RETN 0

}

void naked function YieldGeneral (int arg_0, int arg_1) callsign 72 __asm{
	PUSH 1
	PUSH 2
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	SYSCALL 0x200, (1 | (3 << 16)) ; GetGeneralName
	SYSCALL 0x6, (3 | (0 << 16)) ; 0x0006
	PUSH 0
	PUSH 2
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	SYSCALL 0x200, (1 | (3 << 16)) ; GetGeneralName
	SYSCALL 0x6, (3 | (0 << 16)) ; 0x0006
	PUSHARG -3
	JCOND 1, LOC_36E4
	JCOND 0, LOC_379C
	POP
	JMP LOC_3838
LOC_36E4:
	PUSH 0
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSH 950
	PUSH 966
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSHSTR "SUCCESS"
	PUSH 255
	SYSCALL 0x301, (2 | (0 << 16)) ; PlaySound1
	PUSH 1
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	PUSH 980
	PUSH 988
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSH 0
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSHSTR "%E%"
	SYSCALL 0x3, (3 | (0 << 16)) ; 0x0003
	JMP LOC_3838
LOC_379C:
	PUSH 0
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSH 950
	PUSH 966
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSH 1
	PUSHINV 8 ; INTV_DEFENDER_GENERAL
	PUSH 970
	PUSH 978
	SYSCALL 0x303, (2 | (1 << 16)) ; Rand
	SYSCALL 0xA, (3 | (0 << 16)) ; 0x000A
	PUSH 0
	PUSHINV 7 ; INTV_ATTACKER_GENERAL
	PUSHSTR "%E%"
	SYSCALL 0x3, (3 | (0 << 16)) ; 0x0003
	JMP LOC_3838
LOC_3838:
	PUSH 20
	DELAY
	SYSCALL 0x211, (0 | (0 << 16)) ; 0x0211
	RETN 2

}

void naked function DownBrightness (int arg_0, int arg_1) __asm{
	STACK 1
	SYSCALL 0x130, (0 | (1 << 16)) ; GetBackgroundBrightness
	POPN 1
LOC_3874:
	PUSHARG 1
	PUSHARG -3
	CMPG
	JZ LOC_38C0
	PUSHARG -2
	DELAY
	DECN 1
	PUSHARG 1
	SYSCALL 0x12F, (1 | (0 << 16)) ; SetBackgroundBrightness
	JMP LOC_3874
LOC_38C0:
	PUSHARG -3
	SYSCALL 0x12F, (1 | (0 << 16)) ; SetBackgroundBrightness
	RETN 2

}

void naked function RaiseBrightness (int arg_0, int arg_1) __asm{
	STACK 1
	SYSCALL 0x130, (0 | (1 << 16)) ; GetBackgroundBrightness
	POPN 1
LOC_38F8:
	PUSHARG 1
	PUSHARG -3
	CMPL
	JZ LOC_3944
	PUSHARG -2
	DELAY
	INCN 1
	PUSHARG 1
	SYSCALL 0x12F, (1 | (0 << 16)) ; SetBackgroundBrightness
	JMP LOC_38F8
LOC_3944:
	PUSHARG -3
	SYSCALL 0x12F, (1 | (0 << 16)) ; SetBackgroundBrightness
	RETN 2

}

void naked function Test01 () callsign 98 __asm{
	PUSHINV 2 ; INTV_ATTACKER_MAJOR
	PUSH 0
	PUSH 10000
	SYSCALL 0x125, (3 | (0 << 16)) ; 0x0125
	RETN 0

}

void naked function EnterMapMoveCamera (int arg_0, int arg_1, int arg_2) callsign 58 __asm{
	STACK 7
	INST_09 2
	PUSH 0
	CMP
	JZ LOC_3C6C
	PUSH 1
	SETARG 2
	SYSCALL 0x203, (0 | (1 << 16)) ; 0x0203
	POPN 1
	SYSCALL 0x204, (0 | (1 << 16)) ; 0x0204
	POPN 2
LOC_39E8:
	PUSHARG -4
	PUSHARG 1
	SUB
	POPN 5
	PUSHARG -3
	PUSHARG 2
	SUB
	POPN 6
	PUSHARG 5
	POPN 7
	PUSHARG 7
	PUSH 0
	CMPL
	JZ LOC_3A60
	PUSHARG 7
	NEG
	POPN 7
LOC_3A60:
	PUSHARG 7
	PUSH 2
	DIV
	PUSHARG -2
	CMPL
	JZ LOC_3AAC
	PUSHARG 5
	PUSH 2
	DIV
	POPN 3
	JMP LOC_3AF4
LOC_3AAC:
	PUSHARG 5
	PUSH 0
	CMPG
	JZ LOC_3AE0
	PUSHARG -2
	POPN 3
	JMP LOC_3AF4
LOC_3AE0:
	PUSHARG -2
	NEG
	POPN 3
LOC_3AF4:
	PUSHARG 6
	POPN 7
	PUSHARG 7
	PUSH 0
	CMPL
	JZ LOC_3B34
	PUSHARG 7
	NEG
	POPN 7
LOC_3B34:
	PUSHARG 7
	PUSH 2
	DIV
	PUSHARG -2
	CMPL
	JZ LOC_3B80
	PUSHARG 6
	PUSH 2
	DIV
	POPN 4
	JMP LOC_3BC8
LOC_3B80:
	PUSHARG 6
	PUSH 0
	CMPG
	JZ LOC_3BB4
	PUSHARG -2
	POPN 4
	JMP LOC_3BC8
LOC_3BB4:
	PUSHARG -2
	NEG
	POPN 4
LOC_3BC8:
	PUSHARG 1
	PUSHARG 3
	ADD
	POPN 1
	PUSHARG 2
	PUSHARG 4
	ADD
	POPN 2
	PUSHARG 1
	PUSHARG 2
	SYSCALL 0x205, (2 | (0 << 16)) ; 0x0205
	PUSH 1
	DELAY
	PUSHARG 3
	PUSH 0
	CMPZ
	PUSHARG 4
	PUSH 0
	CMPZ
	ORZ
	JNZ LOC_39E8
	PUSH 0
	SETARG 2
LOC_3C6C:
	RETN 3

}

void naked function MajorDeath (int arg_0, int arg_1) callsign 57 __asm{
	STACK 2
	PUSHARG -3
	PUSHSTR "Yell03"
	PUSH 255
	SYSCALL 0x300, (3 | (0 << 16)) ; PlaySound
	PUSHARG -3
	PUSHSTR "Horse02"
	PUSH 255
	SYSCALL 0x300, (3 | (0 << 16)) ; PlaySound
	PUSHSTR "MoveCamera"
	INST_45
	PUSHARG -3
	SYSCALL 0x105, (1 | (1 << 16)) ; GetObjectScreenX
	POPN 1
	PUSHARG -3
	SYSCALL 0x106, (1 | (1 << 16)) ; GetObjectScreenY
	POPN 2
	PUSHARG 1
	PUSHARG 2
	PUSH 128
	SUB
	PUSH 20
	CALL MoveCamera
	PUSH 10
	DELAY
	PUSHARG -2
	PUSH 0
	CMPZ
	JZ LOC_3D78
	PUSHSTR "VICTORY"
	PUSH 255
	SYSCALL 0x301, (2 | (0 << 16)) ; PlaySound1
LOC_3D78:
	RETN 2

}

void naked function EnterWarprep () callsign 54 __asm{
	PUSHSTR "MENU\\D0_*"
	SYSCALL 0x7, (1 | (0 << 16)) ; BatchLoadShape
	PUSHSTR "MENU\\D1_2A"
	SYSCALL 0x7, (1 | (0 << 16)) ; BatchLoadShape
	PUSHSTR "MENU\\D1_2B"
	SYSCALL 0x7, (1 | (0 << 16)) ; BatchLoadShape
	PUSHSTR "MENU\\D12*"
	SYSCALL 0x7, (1 | (0 << 16)) ; BatchLoadShape
	PUSHSTR "MENU\\WARPREP*"
	SYSCALL 0x7, (1 | (0 << 16)) ; BatchLoadShape
	RETN 0

}

void naked function EnterBattle () callsign 55 __asm{
	PUSHSTR "InitializeBattle"
	PUSH 0
	PUSH 0
	PUSH 0
	PUSH 0
	CALLBS
	RETN 0

}

void naked function EnterMapmode () callsign 53 __asm{
	PUSH 0
	SETARG 2
	PUSHSTR "MENU\\D3*"
	SYSCALL 0x7, (1 | (0 << 16)) ; BatchLoadShape
	PUSHSTR "MENU\\D4*"
	SYSCALL 0x7, (1 | (0 << 16)) ; BatchLoadShape
	PUSHSTR "MENU\\D28"
	SYSCALL 0x7, (1 | (0 << 16)) ; BatchLoadShape
	PUSHSTR "MENU\\D1_2A"
	SYSCALL 0x7, (1 | (0 << 16)) ; BatchLoadShape
	PUSHSTR "MENU\\D1_2B"
	SYSCALL 0x7, (1 | (0 << 16)) ; BatchLoadShape
	PUSHSTR "MENU\\D12*"
	SYSCALL 0x7, (1 | (0 << 16)) ; BatchLoadShape
	RETN 0

}

void naked function EnterPolicy () callsign 52 __asm{
	PUSHSTR "MENU\\S0*"
	SYSCALL 0x7, (1 | (0 << 16)) ; BatchLoadShape
	PUSHSTR "MENU\\C3_B*"
	SYSCALL 0x7, (1 | (0 << 16)) ; BatchLoadShape
	PUSHSTR "MENU\\D1*"
	SYSCALL 0x7, (1 | (0 << 16)) ; BatchLoadShape
	RETN 0
}
