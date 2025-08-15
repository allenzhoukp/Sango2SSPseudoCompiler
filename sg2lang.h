// --------------------
// This "header file" contains all required functions,
// i.e., those to which the compiler will automatically generate function call.
// They are necessary for compiling for pointer operations.
// The idea for getter and setters are from Henryshow's sg2lib.asm.
// Copyright (c) 2018 Allen Zhou
// --------------------



// Since my 1.05+ has introduced all memory operations in the SYSCALL table,
// all the following functions are no longer in use.
// However, if the user is still using the old Sango2 EXE file,
// those functions are needed to be introduced.
// If the user is using non 1.05+ EXE, remember to set use_exe_memory_io = 0.


// I paraphrase the Getter methods to improve efficiency from GetData_NotAligned().
// GetInt() codes are mostly the same as
// the original GetData_NotAligned() of Henryshow,
// but I fixed the bug due to his misusing SAR as SAR.
// I think SetByte() and SetShort() are more rarely used,
// and the efficiency of SetData_NotAligned() is good enough.
// So I don't change them much.

// NOTE since INTV_ARRAY_BASE (0x4A6878) % 4 == 0,
// it makes no difference whether address % 4 or (address - INTV_ARRAY_BASE) % 4 is used.
// however if this address changes, the code may have to be modified.

// NOTE GetData_NotAligned() and SetData_NotAligned() are not here anymore.
// New getter functions for signed byte and signed short are here.
// The default GetByte and GetShort are UNSIGNED, since I think efficiency is more important;
// There's not much could-be-negative fields in Sango2.

#define INTV_ARRAY_BASE	0x4A6878

// Get the 1-byte UNSIGNED integer from the target address.
int naked function GetByte_Old (int address)  __asm {
    PUSHARG address
    PUSH INTV_ARRAY_BASE
    SUB
    PUSH 2
    SAR
    PUSHINVR 0  // data0 = array_base[(address - array_base)]
    PUSHARG address
    PUSH INTV_ARRAY_BASE
    SUB
    PUSH 0x03
    AND
    PUSH 3
    SHL
    SAR
    PUSH 0xFF
    AND         // result = (data0 >> ((address - array_base) % 4 * 8)) & 0xFF;
    RETVAL 1
}

// Get the 1-byte integer from the target address.
int function GetByte_Signed_Old (int address) {
    int result = GetByte_Old (address);
    if(result & 0x80)               //sign bit is 1
        return result | 0xFFFFFF00;
    return result;
}

// Get the 2-byte UNSIGNED integer from the target address.
int naked function GetShort_Old (int address) __asm {
    PUSHARG address
    PUSH 0x03
    AND
    JCOND 3, LANG_GET_SHORT_1 // 如果该2字节横跨在两个DWORD之间，需特殊操作。

    // return (GetINV(minus / 4) >> (minus % 4 * 8)) & 0xFFFF
    PUSHARG address
    PUSH INTV_ARRAY_BASE
    SUB
    PUSH 2
    SAR
    PUSHINVR 0
    PUSHARG address
    PUSH INTV_ARRAY_BASE
    SUB
    PUSH 0x03
    AND
    PUSH 3
    SHL
    SAR
    PUSH 0xFFFF
    AND
    RETVAL 1

LANG_GET_SHORT_1:
    // return ((GetINV(minus / 4 + 1) << 8) | (GetINV(minus / 4) >> 24 & 0xFF)) & 0xFFFF
    PUSHARG address
    PUSH INTV_ARRAY_BASE
    SUB
    PUSH 2
    SAR
    PUSH 1
    ADD
    PUSHINVR 0      //得到下4个字节的值；欲求Short的高位在这一个Int的最低一位
    PUSH 8
    SHL             //最低一位移到第2位
    PUSHARG address
    PUSH INTV_ARRAY_BASE
    SUB
    PUSH 2
    SAR
    PUSHINVR 0      //得到本4个字节的值；欲求Short的低位在这一个Int的最高位
    PUSH 24
    SAR             //最高位移到最低位
    PUSH 0xFF
    AND             //去除较高的3位（去掉有符号右移产生的1）
    OR              //最低位和第2位合并
    PUSH 0xFFFF
    AND             //去除高2位
    RETVAL 1
}

// Get the 2-byte integer from the target address.
int function GetShort_Signed_Old (int address) {
    int result = GetShort_Old (address);
    if(result & 0x8000)               //sign bit is 1
        return result | 0xFFFF0000;
    return result;
}

int naked function GetInt_Old(int address) __asm {
    // GetData_NotAligned has already done well enough, except for the problem of SAR.
    // ...and some litle optimizations.
    // readability? what does that mean?
    // ; stack 2.
    // ; stack 1 : [address]
    // ; stack 2 : [address+4]

    STACK 2

    PUSHARG address
    PUSH INTV_ARRAY_BASE
    SUB
    PUSH 2
    SAR
    PUSHINVR 0
    POPN 1

    PUSHARG address
    // PUSH INTV_ARRAY_BASE
    // SUB
    PUSH 0x03
    AND
    JCOND 0, GET_ADDR_TAIL_V2_0

    PUSHARG address
    PUSH INTV_ARRAY_BASE
    SUB
    PUSH 2
    SAR
    PUSH 1
    ADD
    PUSHINVR 0
    POPN 2

    // JCOND 1, GET_ADDR_TAIL_1
    JCOND 2, GET_ADDR_TAIL_V2_2
    JCOND 3, GET_ADDR_TAIL_V2_3

GET_ADDR_TAIL_V2_1:
    PUSHARG 1
    PUSH 8
    SAR			// stack1 = [address] >> 8;
    PUSH 0xFFFFFF
    AND         // clr high bit
    PUSHARG 2
    PUSH 24
    SHL			// stack2 = [address+4] << 24 ;
    OR
    RETVAL 1

GET_ADDR_TAIL_V2_2:
    PUSHARG 1
    PUSH 16
    SAR			// stack1 = [address] >> 16
    PUSH 0xFFFF
    AND         // clr high bits
    PUSHARG 2
    PUSH 24
    SHL			// stack2 = [address+4] << 16 ;
    OR
    RETVAL 1

GET_ADDR_TAIL_V2_3:
    PUSHARG 1
    PUSH 24
    SAR			// stack1 = [address] >> 24
    PUSH 0xFF
    AND         // clr high bits
    PUSHARG 2
    PUSH 8
    SHL			// stack2 = [address+4] << 8 ;
    OR
    RETVAL 1

GET_ADDR_TAIL_V2_0:
    PUSHARG 1
    RETVAL 1
}

// SetData_NotAligned() has been renamed (and slightly modified)
void naked function SetInt_Old(int address, int value) __asm {
    // ; Parameters:
    // ; arg 0 : return PC
    // ; arg -1 : caller SP
    //
    // ; stack 5.
    // ; stack 1 : address % 4
    // ; stack 2 : [address]
    // ; stack 3 : [address+4];
    // ; stack 4 : returned value.
    // ; stack 5 : tmp

    STACK 5

	PUSHARG address
	PUSH 0x03
	AND
	POPN 1

	PUSHARG address
	PUSH INTV_ARRAY_BASE
	SUB
	PUSH 2
	SAR
	PUSHINVR 0
	POPN 2

	PUSHARG address
	PUSH INTV_ARRAY_BASE
	SUB
	PUSH 2
	SAR
	PUSH 1
	ADD
	PUSHINVR 0
	POPN 3

	PUSHARG 1
	JCOND 0, SET_ADDR_TAIL_0
	JCOND 1, SET_ADDR_TAIL_1
	JCOND 2, SET_ADDR_TAIL_2
	JCOND 3, SET_ADDR_TAIL_3

SET_ADDR_TAIL_0:
	PUSHARG value // 压入要写的数据

	PUSHARG address
	PUSH INTV_ARRAY_BASE
	SUB
	PUSH 2
	SAR	// 压入要写的地址
	SETINVR 0 // 写入数据

	JMP SET_DATA_END

SET_ADDR_TAIL_1:
	PUSHARG value // 压入要写的数据
	PUSH 0xFFFFFF
	AND
	PUSH 8
	SHL
	PUSHARG 2
	PUSH 0xFF
	AND
	OR		//(src & 0xFF) | ((dest & 0xFFFFFF) << 8)
	PUSHARG address
	PUSH INTV_ARRAY_BASE
	SUB
	PUSH 2
	SAR	// 压入要写的地址
	SETINVR 0 // 写入数据

	PUSHARG value // 压入要写的数据
	PUSH 24
	SAR
	PUSHARG 3
	PUSH 0xFFFFFF00
	AND
	OR		//(src & 0xFFFFFF00) | ((dest & 0xFF000000) >> 24)
	PUSHARG address
	PUSH INTV_ARRAY_BASE
	SUB
	PUSH 2
	SAR
	PUSH 1
	ADD // 压入要写的地址
	SETINVR 0 // 写入数据
	JMP SET_DATA_END

SET_ADDR_TAIL_2:
	PUSHARG value // 压入要写的数据
	PUSH 0xFFFF
	AND
	PUSH 16
	SHL
	PUSHARG 2
	PUSH 0xFFFF
	AND
	OR 		//(src & 0xFFFF) | ((dest & 0xFFFF) << 16)
	PUSHARG address
	PUSH INTV_ARRAY_BASE
	SUB
	PUSH 2
	SAR	// 压入要写的地址
	SETINVR 0 // 写入数据

	PUSHARG value // 压入要写的数据
	PUSH 16
	SAR
	PUSHARG 3
	PUSH 0xFFFF0000
	AND
	OR		//(src & 0xFFFF0000) | ((dest & 0xFFFF0000) >> 16)
	PUSHARG address
	PUSH INTV_ARRAY_BASE
	SUB
	PUSH 2
	SAR
	PUSH 1
	ADD // 压入要写的地址
	SETINVR 0 // 写入数据
	JMP SET_DATA_END

SET_ADDR_TAIL_3:
	PUSHARG value // 压入要写的数据
	PUSH 0xFF
	AND
	PUSH 24
	SHL
	PUSHARG 2
	PUSH 0xFFFFFF
	AND
	OR		//(src & 0xFFFFFF) | ((dest & 0xFF) << 24)
	PUSHARG address
	PUSH INTV_ARRAY_BASE
	SUB
	PUSH 2
	SAR	// 压入要写的地址
	SETINVR 0 // 写入数据

	PUSHARG value // 压入要写的数据
	PUSH 8
	SAR
	PUSHARG 3
	PUSH 0xFF000000
	AND
	OR		//(src & 0xFF000000) | ((dest & 0xFFFFFF00) >> 8)
	PUSHARG address
	PUSH INTV_ARRAY_BASE
	SUB
	PUSH 2
	SAR
	PUSH 1
	ADD // 压入要写的地址
	SETINVR 0 // 写入数据
	JMP SET_DATA_END

SET_DATA_END:
	RETN 2 //modified
}


// The following functions are aimed at making memory operations easier.

// NOTE the sign doesnt matters here.

// Write the 2-byte integer value to target address.
// the higher 2 bytes of argument value will be ignored.
void function SetShort_Old(int address, int value) {
    SetInt_Old(address,
        (GetShort_Old(address + 2) << 16) | (value & 0x0000FFFF));
}

// Write the 1-byte integer value to target address.
// the higher 3 bytes of argument value will be ignored.
void function SetByte_Old(int address, int value) {
    SetInt_Old(address,
        (GetInt_Old(address) & 0xFFFFFF00) | (value & 0x000000FF));
}

// Write string (in stack) to target address.
// Use a brute-force style: Target syscall 0x213, always set before use
//                          (no global flag indicates whether it is set).
void function SetString_Old(int address, string value) {
    //0x46DC60 = strcpy
    SetInt_Old(0x4A71FC, 0x46DC60); // 0x4A69B0(SYSCALL_ARRAY_BASE) + 0x213 * 4

    __asm {
        PUSHARG address
        PUSHARG value
        SYSCALL 0x213, (2 | (1 << 16)) ; Modified 0x213 (SetString)
    }
}
