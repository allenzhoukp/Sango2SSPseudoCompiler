// Creator: Allen Zhou
// This "header file" contains all defaultly used functions. They are necessary for compiling.
// e.g. To parse operator * and ->, function call GetInt() and SetInt() is necessary.

// These pair of function is used for reading/writing memory values and was created by HenryShow.

// NOTE: Although HenryShow claims that it works, sometimes only the last byte is correctly read.
// it is NOT SUGGESTED to use it directly.
int function GetData_NotAligned(address) {
    // ; Parameters:
    // ; arg 0 : return PC
    // ; arg -1 : caller SP
    // ; arg -2 : user arg 1
    // ; arg -3 : user arg 2
    // ;
    // ; UserArg1 : Address to read.
    // ;
    // ; Returned Value:
    // ; DWORD
    //
    // ; stack 5.
    // ; stack 1 : address % 4
    // ; stack 2 : [address]
    // ; stack 3 : [address+4];
    // ; stack 4 : returned value.
    // ; stack 5 : tmp
    __asm {
        STACK 5

        PUSHARG -2
        PUSH 0x03
        AND
        POPN 1

        PUSHARG -2
        PUSH INTV_ARRAY_BASE
        SUB
        PUSH 2
        SHR
        PUSHINVR 0
        POPN 2

        PUSHARG -2
        PUSH INTV_ARRAY_BASE
        SUB
        PUSH 2
        SHR
        PUSH 1
        ADD
        PUSHINVR 0
        POPN 3

        PUSHARG 1
        JCOND 0, GET_ADDR_TAIL_0
        JCOND 1, GET_ADDR_TAIL_1
        JCOND 2, GET_ADDR_TAIL_2
        JCOND 3, GET_ADDR_TAIL_3

    GET_ADDR_TAIL_0:
        PUSHARG 2
        POPN 4
        JMP GET_DATA_END

    GET_ADDR_TAIL_1:
        PUSHARG 2
        PUSH 8
        SHR			// stack1 = [address] >> 8;
        PUSHARG 3
        PUSH 24
        SHL			// stack2 = [address+4] << 24 ;
        OR
        POPN 4
        JMP GET_DATA_END

    GET_ADDR_TAIL_2:
        PUSHARG 2
        PUSH 16
        SHR			// stack1 = [address] >> 16
        PUSHARG 3
        PUSH 24
        SHL			// stack2 = [address+4] << 16 ;
        OR
        POPN 4
        JMP GET_DATA_END

    GET_ADDR_TAIL_3:
        PUSHARG 2
        PUSH 24
        SHR			// stack1 = [address] >> 24
        PUSHARG 3
        PUSH 8
        SHL			// stack2 = [address+4] << 8 ;
        OR
        POPN 4
        JMP GET_DATA_END

    GET_DATA_END:
        POP
        PUSHARG 4
        POPN -2
        RETN 0      // 压入1个参数，得到的数据放入参数位置，RETN 0，表明调用堆栈由调用者自行调整。
    }
}

// NOTE: Although this function literally requires argument,
//       the function argument list is empty, due to the fact that it will not affect caller stack.
// e.g. When calling SetData_NotAligned(address, value),
//                   the address AND the value will be remained in the stack.
// Due to this reason, it is NOT SUGGESTED to use it directly.
void function SetData_NotAligned() {
    // ; Parameters:
    // ; arg 0 : return PC
    // ; arg -1 : caller SP
    // ; arg -2 : user arg 1
    // ; arg -3 : user arg 2
    // ; arg -4 : user arg 3
    // ;
    // ; UserArg1 : Address.
    // ; UserArg2 : Data
    // ;
    // ; Returned Value:
    // ; DWORD
    //
    // ; stack 5.
    // ; stack 1 : address % 4
    // ; stack 2 : [address]
    // ; stack 3 : [address+4];
    // ; stack 4 : returned value.
    // ; stack 5 : tmp
    __asm {
        STACK 5

    	PUSHARG -2
    	PUSH 0x03
    	AND
    	POPN 1

    	PUSHARG -2
    	PUSH INTV_ARRAY_BASE
    	SUB
    	PUSH 2
    	SHR
    	PUSHINVR 0
    	POPN 2

    	PUSHARG -2
    	PUSH INTV_ARRAY_BASE
    	SUB
    	PUSH 2
    	SHR
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
    	PUSHARG -3 // 压入要写的数据

    	PUSHARG -2
    	PUSH INTV_ARRAY_BASE
    	SUB
    	PUSH 2
    	SHR	// 压入要写的地址
    	INST_53 0 // 写入数据

    	JMP SET_DATA_END

    SET_ADDR_TAIL_1:
    	PUSHARG -3 // 压入要写的数据
    	PUSH 0xFFFFFF
    	AND
    	PUSH 8
    	SHL
    	PUSHARG 2
    	PUSH 0xFF
    	AND
    	OR		//(src & 0xFF) | ((dest & 0xFFFFFF) << 8)
    	PUSHARG -2
    	PUSH INTV_ARRAY_BASE
    	SUB
    	PUSH 2
    	SHR	// 压入要写的地址
    	INST_53 0 // 写入数据

    	PUSHARG -3 // 压入要写的数据
    	PUSH 24
    	SHR
    	PUSHARG 3
    	PUSH 0xFFFFFF00
    	AND
    	OR		//(src & 0xFFFFFF00) | ((dest & 0xFF000000) >> 24)
    	PUSHARG -2
    	PUSH INTV_ARRAY_BASE
    	SUB
    	PUSH 2
    	SHR
    	PUSH 1
    	ADD // 压入要写的地址
    	INST_53 0 // 写入数据
    	JMP SET_DATA_END

    SET_ADDR_TAIL_2:
    	PUSHARG -3 // 压入要写的数据
    	PUSH 0xFFFF
    	AND
    	PUSH 16
    	SHL
    	PUSHARG 2
    	PUSH 0xFFFF
    	AND
    	OR 		//(src & 0xFFFF) | ((dest & 0xFFFF) << 16)
    	PUSHARG -2
    	PUSH INTV_ARRAY_BASE
    	SUB
    	PUSH 2
    	SHR	// 压入要写的地址
    	INST_53 0 // 写入数据

    	PUSHARG -3 // 压入要写的数据
    	PUSH 16
    	SHR
    	PUSHARG 3
    	PUSH 0xFFFF0000
    	AND
    	OR		//(src & 0xFFFF0000) | ((dest & 0xFFFF0000) >> 16)
    	PUSHARG -2
    	PUSH INTV_ARRAY_BASE
    	SUB
    	PUSH 2
    	SHR
    	PUSH 1
    	ADD // 压入要写的地址
    	INST_53 0 // 写入数据
    	JMP SET_DATA_END

    SET_ADDR_TAIL_3:
    	PUSHARG -3 // 压入要写的数据
    	PUSH 0xFF
    	AND
    	PUSH 24
    	SHL
    	PUSHARG 2
    	PUSH 0xFFFFFF
    	AND
    	OR		//(src & 0xFFFFFF) | ((dest & 0xFF) << 24)
    	PUSHARG -2
    	PUSH INTV_ARRAY_BASE
    	SUB
    	PUSH 2
    	SHR	// 压入要写的地址
    	INST_53 0 // 写入数据

    	PUSHARG -3 // 压入要写的数据
    	PUSH 8
    	SHR
    	PUSHARG 3
    	PUSH 0xFF000000
    	AND
    	OR		//(src & 0xFF000000) | ((dest & 0xFFFFFF00) >> 8)
    	PUSHARG -2
    	PUSH INTV_ARRAY_BASE
    	SUB
    	PUSH 2
    	SHR
    	PUSH 1
    	ADD // 压入要写的地址
    	INST_53 0 // 写入数据
    	JMP SET_DATA_END

    SET_DATA_END:
    	RETN 0 // 压入1个参数，得到的数据放入参数位置，RETN 0，表明调用堆栈由调用者自行调整。
    }
}


// Creator: Allen Zhou
// The following functions are aimed at making memory operations easier.
// NOTE the old functions (GetData, GetData_BYTE, GetData_WORD, etc.) are not in this file any more.

// Get a 4-byte integer data from the address.
int function GetInt(address) {
    return (GetData_NotAligned(address) & 0x000000FF) |
          ((GetData_NotAligned(address + 1) & 0x000000FF) << 8) |
          ((GetData_NotAligned(address + 2) & 0x000000FF) << 16) |
          ((GetData_NotAligned(address + 3) & 0x000000FF) << 24);
}

// Get a 1-byte UNSIGNED integer data from the address.
int function GetByte(address) {
    return GetData_NotAligned(address) & 0x000000FF;
}

// Get a 2-byte UNSIGNED integer data from the address.
int function GetShort(address) {
    return (GetData_NotAligned(address) & 0x000000FF) |
          ((GetData_NotAligned(address + 1) & 0x000000FF) << 8);
}

// Write the 4-byte integer value to target address.
void function SetInt(address, value) {
    SetData_NotAligned(address, value);
}

// Write the 2-byte UNSIGNED integer value to target address.
// the higher 2 bytes of argument value will be ignored.
void function SetShort(address, value) {
    SetData_NotAligned(address,
        (GetShort(address + 2) << 16) | (value & 0x0000FFFF));
}

// Write the 1-byte UNSIGNED integer value to target address.
// the higher 3 bytes of argument value will be ignored.
void function SetByte(address, value) {
    SetData_NotAligned(address,
        (GetInt(address) & 0xFFFFFF00) | (value & 0x000000FF));
}
