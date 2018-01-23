include "predefine.inc"
org 40h

Sol13LongSpcSmokeFade:
	STACK 3
	PUSHARG -2 ; smoke
	PUSHSTR 1 ; "S013SND01"
	PUSH 255
	SYSCALL 0x300, (3 | (0 << 16)) ; PlaySound
	PUSH 80
	DELAY
	PUSHARG -2 ; smoke
	PUSH 262144
	SYSCALL 0x29, (2 | (0 << 16)) ; SetObjectFlags
	PUSHARG -2 ; smoke
	PUSH 16
	SYSCALL 0x27, (2 | (0 << 16)) ; SetObjectBrightness
	PUSH 16
	POPN 3 ; brightness
t1516723692_1_for:
	PUSHARG 3 ; brightness
	PUSH 0
	CMPG
	JZ t1516723692_3_for_end
	PUSH 3
	DELAY
	PUSHARG -2 ; smoke
	PUSHARG 3 ; brightness
	SYSCALL 0x27, (2 | (0 << 16)) ; SetObjectBrightness
t1516723692_2_for_continue:
	DECN 3 ; brightness
	JMP t1516723692_1_for
t1516723692_3_for_end:
	PUSHARG -2 ; smoke
	SYSCALL 0x13, (1 | (0 << 16)) ; FreeObjectByHandle
	RETN 1

Sol13LongSpcAttack:
	STACK 25
	SYSCALL 0xB, (0 | (1 << 16)) ; GetScriptLinkedObject
	POPN 3 ; soldier
	PUSHARG 3 ; soldier
	SYSCALL 0x1A, (1 | (1 << 16)) ; GetObjectDir
	POPN 4 ; soldierDir
	PUSHARG 4 ; soldierDir
	PUSH 0
	CMP
	JZ t1516723692_6_else
t1516723692_5_if:
	PUSH 1
	POPN 4 ; soldierDir
	JMP t1516723692_7_if_end
t1516723692_6_else:
	PUSH 1
	NEG
	POPN 4 ; soldierDir
t1516723692_7_if_end:
	PUSHARG 3 ; soldier
	SYSCALL 0x105, (1 | (1 << 16)) ; GetObjectScreenX
	POPN 5 ; soldierScreenX
	PUSHARG 3 ; soldier
	SYSCALL 0x106, (1 | (1 << 16)) ; GetObjectScreenY
	POPN 6 ; soldierScreenY
	PUSH 1
	POPN 13 ; i
t1516723692_8_for:
	PUSHARG 13 ; i
	PUSHARG -2 ; shootRange
	CMPLE
	JZ t1516723692_10_for_end
	PUSHARG 5 ; soldierScreenX
	SYSCALL 0x108, (1 | (1 << 16)) ; ScreenXToBattleX
	PUSHARG 4 ; soldierDir
	PUSHARG 13 ; i
	MUL
	ADD
	PUSHARG 6 ; soldierScreenY
	SYSCALL 0x109, (1 | (1 << 16)) ; ScreenYToBattleY
	SYSCALL 0x113, (2 | (1 << 16)) ; GetSoldierHandle
	POPN 14 ; target
	PUSHARG 14 ; target
	PUSH 0
	CMP
	JZ t1516723692_12_else
t1516723692_11_if:
	PUSH 0
	PUSHARG 13 ; i
	PUSH 1
	SUB
	SETNR 7 ; log
	JMP t1516723692_9_for_continue
t1516723692_12_else:
t1516723692_13_if_end:
	PUSH 1
	PUSHARG 13 ; i
	PUSH 1
	SUB
	SETNR 7 ; log
	PUSHARG 14 ; target
	SYSCALL 0x105, (1 | (1 << 16)) ; GetObjectScreenX
	POPN 15 ; targetScreenX
	PUSHARG 14 ; target
	SYSCALL 0x106, (1 | (1 << 16)) ; GetObjectScreenY
	POPN 16 ; targetScreenY
	PUSHARG 15 ; targetScreenX
	PUSHARG 16 ; targetScreenY
	PUSH 0
	PUSH 0
	PUSH 504
	SYSCALL 0x10, (5 | (1 << 16)) ; CreateObjectRaw
	POPN 17 ; smoke
	PUSHSTR 0 ; "Sol13LongSpcSmokeFade"
	PUSHARG 17 ; smoke
	PUSH 0
	PUSH 0
	PUSH 0
	CALLBS
	JMP t1516723692_10_for_end
t1516723692_9_for_continue:
	INCN 13 ; i
	JMP t1516723692_8_for
t1516723692_10_for_end:
	PUSHARG 14 ; target
	PUSH 0
	CMP
	JZ t1516723692_15_else
t1516723692_14_if:
	RETN 1
t1516723692_15_else:
t1516723692_16_if_end:
	PUSHARG 14 ; target
	SYSCALL 0x1A, (1 | (1 << 16)) ; GetObjectDir
	POPN 18 ; targetDir
	PUSHARG 14 ; target
	SYSCALL 0x134, (1 | (0 << 16)) ; KillSoldier
	PUSHARG 3 ; soldier
	SYSCALL 0x115, (1 | (1 << 16)) ; GetSoldierSide
	POPN 19 ; soldierSide
	PUSH 4875424
	POPN 20 ; pObjectListRoot
	PUSHARG 19 ; soldierSide
	SYSCALL 0x10C, (1 | (1 << 16)) ; GetSoldierCount
	PUSH 200
	CMP
	ZERO
	JZ t1516723692_18_else
t1516723692_17_if:
	PUSHARG 20 ; pObjectListRoot
	CALL GetInt
	PUSHARG 3 ; soldier
	PUSH 65535
	AND
	PUSH 168
	MUL
	PUSH 132
	ADD
	ADD
	CALL GetInt
	PUSH 0
	ADD
	CALL GetInt
	POPN 21 ; pSelfGeneral
	PUSHARG 20 ; pObjectListRoot
	CALL GetInt
	PUSHARG 14 ; target
	PUSH 65535
	AND
	PUSH 168
	MUL
	PUSH 132
	ADD
	ADD
	CALL GetInt
	PUSH 0
	ADD
	CALL GetInt
	POPN 22 ; pEnemyGeneral
	PUSHARG 21 ; pSelfGeneral
	PUSH 139
	ADD
	CALL GetInt
	POPN 23 ; selfSoldierType
	PUSHARG 22 ; pEnemyGeneral
	PUSH 139
	ADD
	CALL GetInt
	POPN 24 ; enemySoldierType
	PUSH 20
	PUSHARG 14 ; target
	SYSCALL 0x2D, (1 | (1 << 16)) ; GetObjectSequence
	INST_53 0
	PUSH 19
	PUSH 1
	INST_53 0
	PUSHARG 21 ; pSelfGeneral
	PUSH 139
	ADD
	PUSHARG 24 ; enemySoldierType
	CALL SetInt
	PUSHARG 20 ; pObjectListRoot
	CALL GetInt
	PUSHARG 3 ; soldier
	PUSH 65535
	AND
	PUSH 168
	MUL
	PUSH 132
	ADD
	ADD
	CALL GetInt
	PUSH 44
	ADD
	CALL GetInt
	PUSH 4
	AND
	JZ t1516723692_21_else
t1516723692_20_if:
	PUSHARG 19 ; soldierSide
	PUSHARG 15 ; targetScreenX
	SYSCALL 0x108, (1 | (1 << 16)) ; ScreenXToBattleX
	PUSH 10000
	ADD
	PUSHARG 16 ; targetScreenY
	SYSCALL 0x109, (1 | (1 << 16)) ; ScreenYToBattleY
	SYSCALL 0x119, (3 | (1 << 16)) ; CreateSoldier
	POPN 25 ; newSoldier
	JMP t1516723692_22_if_end
t1516723692_21_else:
	PUSHARG 19 ; soldierSide
	PUSHARG 15 ; targetScreenX
	SYSCALL 0x108, (1 | (1 << 16)) ; ScreenXToBattleX
	PUSHARG 16 ; targetScreenY
	SYSCALL 0x109, (1 | (1 << 16)) ; ScreenYToBattleY
	SYSCALL 0x119, (3 | (1 << 16)) ; CreateSoldier
	POPN 25 ; newSoldier
t1516723692_22_if_end:
	PUSHARG 25 ; newSoldier
	PUSHARG 15 ; targetScreenX
	PUSHARG 16 ; targetScreenY
	PUSH 0
	SYSCALL 0x14, (4 | (0 << 16)) ; SetObjectCoordinate
	PUSHARG 25 ; newSoldier
	PUSHARG 18 ; targetDir
	SYSCALL 0x30, (2 | (0 << 16)) ; SetObjectDir
	PUSHARG 21 ; pSelfGeneral
	PUSH 139
	ADD
	PUSHARG 23 ; selfSoldierType
	CALL SetInt
	PUSH 0
	PUSH 20
	INST_53 0
	PUSH 0
	PUSH 19
	INST_53 0
	PUSHARG 14 ; target
	SYSCALL 0x13, (1 | (0 << 16)) ; FreeObjectByHandle
t1516723692_18_else:
t1516723692_19_if_end:
	RETN 1

STRING_0: DB "Sol13LongSpcSmokeFade"
STRING_1: DB "S013SND01"
STRING_2: DB "Sol13LongSpcAttack"

magic_table_addr:
MAGIC_0: procedure Sol13LongSpcSmokeFade 1 STRING_0 0
MAGIC_1: procedure Sol13LongSpcAttack 1 STRING_2 29013

magic_table_end:

string_table_addr:
DB STRING_0
DB STRING_1
DB STRING_2

string_table_end:
file_end:
DB "Script Object Creator: Henryshow"
DB "Pseudo Compiler Author: Allen Zhou"
