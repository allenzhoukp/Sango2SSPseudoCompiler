;;远帆：双分号是我的注释，单分号是注册的。
procedure macro addr, initstack, description, sequence
	DB addr
	DB initstack
	DB description
	DB sequence
endm

GETUSERVALUE macro index
	DB 5BH
	DB index
endm

SETUSERVALUE_I macro index, value
	PUSH value
	DB 5AH
	DB index
endm

SETUSERVALUE macro index
	DB 5AH
	DB index
endm

;;Object Flags. 不知道的可以参考我的Things.ini心得。
;;对XX有效的Flags是用在武将技物件上的，EXE会藉由这些Flags判定武将技的命中，命中时调用设置好的Callback函数，同时造成士兵死亡。

;;所有物件，按持有的引用可分为指针（物件Node的地址）和Handle。在脚本中，所有SYSCALL都是用Handle来操作的，这已经可以解决大部分问题。
;;实在用SYSCALL解决不了的，才考虑拿到指针，直接修改内存中的数据。拿到指针的方法参见sg2lib.asm。
OF_MAN                  EQU 0x00000001
OF_MISSILE              EQU 0x00000002
OF_BACKGROUND			EQU 0x00000004
OF_CAVALRY				EQU 0x00000008
OF_BLOCKMAN             EQU 0x00000010
OF_BLOCKMISSILE         EQU 0x00000020
OF_BIGSHAPE				EQU 0x00000040	;特殊放大的物件
OF_TRIGGERMAN           EQU 0x00000100
OF_TRIGGERMISSILE       EQU 0x00000200
OF_NOGRAVITY            EQU 0x00010000  ;没有重力
OF_NONSYNCHRONISM       EQU 0x00020000  ;非同步，当物件产生时会使物件的图不同步
OF_MIXER                EQU 0x00040000	;物件的图形用混色的方式秀图
OF_WHITELIGHT           EQU 0x00080000	;物件的图形用加色的方式秀图
OF_DARKLIGHT            EQU 0x00100000	;物件的图形用减色的方式秀图
OF_ENEMYGENERAL			EQU 0x01000000	;对敌方的武将有效
OF_ENEMYFORCE			EQU 0x02000000	;对敌方的小兵有效
OF_MYGENERAL			EQU 0x04000000	;对我方的武将有效
OF_MYFORCE				EQU 0x08000000	;对我方的小兵有效
OF_ATTACKENEMY			EQU 0x03000000	;只对敌方有效
OF_ATTACKMY				EQU 0x0c000000	;只对我方有效
OF_ATTACKALL			EQU 0x0F000000	;对敌我双方都有效
OF_TARGET				EQU 0x00200000	;有目标，一旦打中目标就爆炸
OF_BOMB					EQU 0x00400000	;只要打中任何一个物件(有效物件)就爆炸

INTV_ARRAY_BASE			EQU	0x4A6878

INTV_PERIOD					EQU	0x00 ; Current Period. ;时期
INTV_ATTACKER_MAJOR		EQU	0x02		; major handle
INTV_DEFENDER_MAJOR		EQU 0x03		; major handle
INTV_MAGIC_ATTACK_VALUE	EQU 0x04
INTV_IS_LEFT			EQU 0x05
INTV_ATTACKER_GENERAL	EQU 0x07		; pGeneral
INTV_DEFENDER_GENERAL	EQU 0x08		; pGeneral
INTV_ATTACKER_KING_GENERAL	EQU 0x09
INTV_USER_0				EQU	0x10
INTV_USER_1				EQU	0x11

;;命名尽量与idb中一致，但无法尽改之，莫怪。

SYSFUNC_TYPE_NO_RETURN_VALUE	EQU	0
SYSFUNC_TYPE_PUSH_RETURN_VALUE	EQU 2
SYSFUNC_TYPE_WAIT_IDLE		EQU 3
INTV_CURRENT_YEAR		EQU (0x4BD390 - INTV_ARRAY_BASE) >> 2
INTV_CURRENT_TICK		EQU	(0x4C30E0 - INTV_ARRAY_BASE) >> 2
INTV_PLAYER_KING		EQU	(0x4A64F0 - INTV_ARRAY_BASE) >> 2
;;加一个盗贼
INTV_ROGUE_KING		EQU	(0x4A64F4 - INTV_ARRAY_BASE) >> 2

PTROOP_START	EQU	0x4A6630
PTROOP_NEXT_VALID	EQU	0x4A6634
PTROOP_TAIL	EQU	0x4A6638
NINTERNAL_COUNT	EQU	0x48E3A8
PSELECTED_TROOP	EQU	0x4C30D4

POBJECT_LIST_ROOT	EQU	0x4A64A0

;;吐个槽：注册的命名有点逼死强迫症，索性把NODE全删了。
;;各个ROOT都是指针类型，也就是PGENERAL_ROOT的【内容】是第一个GENERAL的【地址】，其余类推。（链表的头指针）
PGENERAL_ROOT	EQU	0x4A6180
PCITY_ROOT	EQU	0x4A6170
PKING_ROOT 	EQU	0x4A6190

;;两个都是BATTLE_USER_INFO类型，由于是全局变量，这两个NODE就在0x4B03A0和0x4A8A60处。
LEFT_USER_INFO		EQU	0x4B03A0
RIGHT_USER_INFO		EQU	0x4A8A60

;;城池和武将总数
TOTAL_CITY_COUNT	EQU	0x40	;64
TOTAL_GENERAL_COUNT	EQU	0x191	;401

;;屏幕坐标到战场坐标的换算单位。根据SYSCALL 0x108和0x109得来的。
BATTLE_COORD_X	EQU	0x60
BATTLE_COORD_Y	EQU	0x48

/*
;;原版
GENERAL_STRUCT_SIZE	EQU	0xF7
CITY_STRUCT_SIZE	EQU	0xAA
ITEM_STRUCT_SIZE	EQU	0x56
OFFSET_KING_ITEMS	EQU	0x134
*/
/*
;;注册V0.522
GENERAL_STRUCT_SIZE	EQU	0x0258
CITY_STRUCT_SIZE	EQU	0x1D8
ITEM_STRUCT_SIZE	EQU	0xCC
OFFSET_KING_ITEMS	EQU	0x34A
*/
;;注册V0.519
GENERAL_STRUCT_SIZE	EQU	0x0218
CITY_STRUCT_SIZE	EQU	0x1D8
ITEM_STRUCT_SIZE	EQU	0x86
OFFSET_KING_ITEMS	EQU	0x134


;;这里留下了原有注册的东西。因为命名习惯不同，我记不住注册命的名字，查OFFSET都是翻idb的，所以其他结构体就不列在这里了。
;;可以在idb的Structures选项卡里查到各个结构体的各变量名称和位置，注册的工作做得很详尽了。
;;唯一一个比较头疼的是性别和Magic9（原版所无），经cheat engine确认，加在这里。
;;使用的时候千万要注意变量是几个字节的（BYTE=1, WORD=2, DWORD=4），使用合适的函数。

;;均为一个字节
OFFSET_GENERAL_SEX	EQU	0xF7
OFFSET_GENERAL_MAGIC9	EQU	0xF8

; offset of pGeneral structure.
OFFSET_GENERAL_NAME		EQU	0x00
OFFSET_GENERAL_FACE_SHP	EQU	0x0C
OFFSET_GENERAL_HP_MAX	EQU	0x10
OFFSET_GENERAL_HP		EQU	0x12
OFFSET_GENERAL_MP_MAX	EQU	0x14
OFFSET_GENERAL_MP		EQU	0x16
OFFSET_GENERAL_HP_BASE	EQU	0x18
OFFSET_GENERAL_MP_BASE	EQU	0x1A
OFFSET_GENERAL_EXP		EQU	0x2A
OFFSET_GENERAL_LEVEL	EQU	0x2E
OFFSET_GENERAL_STR_SRC	EQU	0x30
OFFSET_GENERAL_INT_SRC	EQU	0x32
OFFSET_GENERAL_STR		EQU	0x34
OFFSET_GENERAL_INT		EQU	0x36
OFFSET_GENERAL_MORALE	EQU	0x38
OFFSET_GENERAL_FATIGE	EQU	0x3A
OFFSET_GENERAL_LOYAL	EQU	0x3C
OFFSET_GENERAL_INFANTRY_MAX	EQU	0x42
OFFSET_GENERAL_INFANTRY	EQU	0x44
OFFSET_GENERAL_CAVALIER_MAX	EQU	0x46
OFFSET_GENERAL_CAVALIER	EQU	0x48
OFFSET_GENERAL_TROOP_TYPE	EQU	0x8B
/*
	WORD wUnknown3;		// 3E - 3F
	WORD wUnknown4;		// 40 - 41
	WORD wInfantryMax;	// 42 - 43
	WORD wInfantryCount;	//44 - 45
	WORD wCavalierMax;		// 46 - 47
	WORD wCavalierCount;	// 48 - 49
	WORD wPos[5];		// 4A - 53			// 武将在哪几座城市出现。如果&0x2000 == 0x2000则表示本位置未登   ;;（直接设置为0x2000即可）
	UCHAR chMagic[8];		// 54 - 5C
	UCHAR chWise[8];		// 5C - 63
	UCHAR chSelectWise;		// 64 - 64
	LEVEL_INDEX skillLvIndex[8];
	LEVEL_INDEX wiseLvIndex[8];
	WORD wUnknown6;
	DWORD dwValidTroopType;	// 87 - 8A
	DWORD dwCurrentTroopType;	// 8B - 8E
	WORD nType ;				// 8F - 90
	DWORD dwStatus;				// 91 - 94
	WORD field_95;				// 95 - 96
	WORD field_97;				// 97 - 98
	WORD wItemHorse;			// 99 - 9A
	WORD wItemWeapon;			// 9B - 9C
	WORD wItemHold;				// 9D - 9E
	DWORD field_9F;
	DWORD field_A3;
	DWORD dwCanUseBattleArray;  // 物品+42H可以调整这个栏位。 阵型？
	DWORD field_AB;
	DWORD field_AF;
	DWORD field_B3;
	DWORD field_B7;
	DWORD field_BB;
	struct _KING_NODE* pKing1;        // BF
	struct _GENERAL_NODE * pBloodGeneral;  // C3
	DWORD nConstancy;  // C7
	struct _KING_NODE* pKing;   // CB
	DWORD field_CF;				// CF
	struct _TROOP_NODE* pTroop;					// D3
	struct _CITY_NODE* pCity;					// D7
	struct _GENERAL_NODE* pNextSameKing;		// DB
	struct _GENERAL_NODE* pNextSameTroop;		// DF
	struct _GENERAL_NODE* pNextSameCity;      // E3
	struct _GENERAL_NODE* pNextSameOuttaCity;		// E7 在野
	DWORD field_EB;
	DWORD field_EF;
} GENERAL, *PGENERAL;
*/

; offset of pKing structure
OFFSET_KING_NAME			EQU	0x00
OFFSET_KING_PGENERAL		EQU	0x0C
OFFSET_KING_PGENERAL_LIST_HEAD	EQU	0x18

magic_table_count	equ	(magic_table_end - magic_table_addr) / 0x10
string_table_count	equ	(string_table_end - string_table_addr) / 0x04
org 00h
	db "Smart Script"		; signature
org 10h
	db 0x00000100 			; version
	db 0x00000040			; size
	db magic_table_addr 	;
	db magic_table_count	;
	db string_table_addr	;
	db string_table_count	;
	db 0x79					;not known yet. but this dword is used to allocate a buffer which is stored to MainScript.field_18
	db file_end				; file length
