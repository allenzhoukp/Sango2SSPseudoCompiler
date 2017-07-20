
; Parameters:
; arg 0 : return PC
; arg -1 : caller SP
; arg -2 : user arg 1
; arg -3 : user arg 2
; 
; UserArg1 : Address to read.
;
; Returned Value:
; DWORD

; stack 5.
; stack 1 : address % 4
; stack 2 : [address]
; stack 3 : [address+4];
; stack 4 : returned value.
; stack 5 : tmp

GetData_NotAligned:
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
	SHR			; stack1 = [address] >> 8;
	PUSHARG 3
	PUSH 24
	SHL			; stack2 = [address+4] << 24 ;
	OR
	POPN 4
	JMP GET_DATA_END

GET_ADDR_TAIL_2:
	PUSHARG 2
	PUSH 16
	SHR			; stack1 = [address] >> 16
	PUSHARG 3
	PUSH 24
	SHL			; stack2 = [address+4] << 16 ;
	OR
	POPN 4
	JMP GET_DATA_END

GET_ADDR_TAIL_3:
	PUSHARG 2
	PUSH 24
	SHR			; stack1 = [address] >> 24
	PUSHARG 3
	PUSH 8
	SHL			; stack2 = [address+4] << 8 ;
	OR
	POPN 4
	JMP GET_DATA_END
	
GET_DATA_END:
	POP
	PUSHARG 4
	POPN -2
	RETN 0 ; 压入1个参数，得到的数据放入参数位置，RETN 0，表明调用堆栈由调用者自行调整。
	
	

; Parameters:
; arg 0 : return PC
; arg -1 : caller SP
; arg -2 : user arg 1
; arg -3 : user arg 2
; arg -4 : user arg 3
; 
; UserArg1 : Address.
; UserArg2 : Data
;
; Returned Value:
; DWORD

; stack 5.
; stack 1 : address % 4
; stack 2 : [address]
; stack 3 : [address+4];
; stack 4 : returned value.
; stack 5 : tmp	
SetData_NotAligned:
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
	PUSHARG -3 ; 压入要写的数据
	
	PUSHARG -2
	PUSH INTV_ARRAY_BASE
	SUB
	PUSH 2
	SHR	; 压入要写的地址
	INST_53 0 ; 写入数据
	
	JMP SET_DATA_END

SET_ADDR_TAIL_1:
	PUSHARG -3 ; 压入要写的数据
	PUSH 0xFFFFFF
	AND
	PUSH 8
	SHL
	PUSHARG 2
	PUSH 0xFF
	AND
	OR		;(src & 0xFF) | ((dest & 0xFFFFFF) << 8)	
	PUSHARG -2
	PUSH INTV_ARRAY_BASE
	SUB
	PUSH 2
	SHR	; 压入要写的地址
	INST_53 0 ; 写入数据

	PUSHARG -3 ; 压入要写的数据
	PUSH 24
	SHR
	PUSHARG 3
	PUSH 0xFFFFFF00
	AND
	OR		;(src & 0xFFFFFF00) | ((dest & 0xFF000000) >> 24)
	PUSHARG -2
	PUSH INTV_ARRAY_BASE
	SUB
	PUSH 2
	SHR	
	PUSH 1
	ADD ; 压入要写的地址
	INST_53 0 ; 写入数据
	JMP SET_DATA_END

SET_ADDR_TAIL_2:
	PUSHARG -3 ; 压入要写的数据
	PUSH 0xFFFF
	AND
	PUSH 16
	SHL
	PUSHARG 2
	PUSH 0xFFFF
	AND
	OR 		;(src & 0xFFFF) | ((dest & 0xFFFF) << 16)
	PUSHARG -2
	PUSH INTV_ARRAY_BASE
	SUB
	PUSH 2
	SHR	; 压入要写的地址
	INST_53 0 ; 写入数据

	PUSHARG -3 ; 压入要写的数据
	PUSH 16
	SHR
	PUSHARG 3
	PUSH 0xFFFF0000
	AND
	OR		;(src & 0xFFFF0000) | ((dest & 0xFFFF0000) >> 16)
	PUSHARG -2
	PUSH INTV_ARRAY_BASE
	SUB
	PUSH 2
	SHR	
	PUSH 1
	ADD ; 压入要写的地址
	INST_53 0 ; 写入数据
	JMP SET_DATA_END

SET_ADDR_TAIL_3:
	PUSHARG -3 ; 压入要写的数据
	PUSH 0xFF
	AND
	PUSH 24
	SHL
	PUSHARG 2
	PUSH 0xFFFFFF
	AND
	OR		;(src & 0xFFFFFF) | ((dest & 0xFF) << 24)
	PUSHARG -2
	PUSH INTV_ARRAY_BASE
	SUB
	PUSH 2
	SHR	; 压入要写的地址
	INST_53 0 ; 写入数据

	PUSHARG -3 ; 压入要写的数据
	PUSH 8
	SHR
	PUSHARG 3
	PUSH 0xFF000000
	AND
	OR		;(src & 0xFF000000) | ((dest & 0xFFFFFF00) >> 8)
	PUSHARG -2
	PUSH INTV_ARRAY_BASE
	SUB
	PUSH 2
	SHR	
	PUSH 1
	ADD ; 压入要写的地址
	INST_53 0 ; 写入数据
	JMP SET_DATA_END
	
SET_DATA_END:
	RETN 0 ; 压入1个参数，得到的数据放入参数位置，RETN 0，表明调用堆栈由调用者自行调整。
