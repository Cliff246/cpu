#ifndef __FLAGS__
#define __FLAGS__



#define WCPU_PATHS_LIST(X)\
	X(ALU, alu, 0)		  \
	X(MEM, mem, 1)		  \
	X(JMP, jmp, 2)		  \
	X(SYS, sys, 3)		  \
	X(FPU, fpu, 4)	      \
	X(ATM, atm, 5)		  \
	X(VEC, vec, 6)		  \

//clone of c_cpu flags since i am lazy

//current paths


//alu path
#define PATH_ALU 0
//memory path
#define PATH_MEM 1
//jump path
#define PATH_JMP 2
//system path
#define PATH_SYS 3
//path floating point unit
#define PATH_FPU 4
//path atomic
#define PATH_ATM 5

#define PATH_VEC 6


//acc_mode
//lane1 = lhs
//lane2 = rhs
//lane3 = other


//

//instruction convention is always
//3			7		6		6		  6	     1 	       1	      1              1
//[31:28] [27:21] [20:15] [15:10]   [9:4]   [3] 	  [2]        [1]   		   [0]
//path    subpath  rd      rs1     rs2     accflag	selflag  reallocflag        immf


//realloc has the associated immediate turn into a key thats reallocable. so you can do dynamic code with constants
//selflag turns rs2 into a number... as in, dont load rs2 convert it into a 6 bit number



//immf 00 = NON



//ALL internal operations that are get and set are done via rd and rs1.

// as in get is just setting rd,
// as in set is just setting rs1

//so rd = x0, rs1 = x0 = nop
//so rd != x0, rs1 = x0 = get
//so rd = x0, rs1 != x0 = set
//so rd != x0, rs1 != x0 = exchange

//PATH CAP, PATH LOWER, OPCODE CLASS UPPER, OPCODE CLASS LOWER, NUMBER, token, base impl, operator, equation, description, unsigned at end
#define WCPU_SUBPATH_ALU_LIST(X)\
	X(ALU,alu,ADD		, add		, 0x00, add			, T0	, "+"		,	"dst = lhs + rhs + oth"				, "arithmetic add"						, 0)				\
	X(ALU,alu,SUB		, sub		, 0x01, sub			, T0	, "-"		,	"dst = lhs - rhs + oth"				, "arithmetic subtract"					, 0)				\
	X(ALU,alu,SUBU		, subu		, 0x02, subu		, T0	, "-"		,	"dst = (uint)lhs - (uint)rhs + oth"	, "arithmetic subtrace unsigned"		, 1)				\
	X(ALU,alu,AND		, and		, 0x03, and			, T0	, "&"		,	"dst = lhs & rhs + oth"				, "logical and"							, 0)				\
	X(ALU,alu,OR		, or		, 0x04, or			, T0	, "|"		,	"dst = lhs | rhs + oth"				, "logical or"							, 0)				\
	X(ALU,alu,XOR		, xor		, 0x05, xor			, T0	, "^"		,	"dst = lhs ^ rhs + oth"				, "logical xor"							, 0)				\
	X(ALU,alu,SLL		, sll		, 0x06, sll			, T0	, "<<"		,	"dst = lhs << rhs + oth"			, "logical shift left"					, 0)				\
	X(ALU,alu,SRL		, srl		, 0x07, srl			, T0	, ">>"		,	"dst = lhs >> rhs + oth"			, "logical shift right"					, 0)				\
	X(ALU,alu,SRA		, sra		, 0x08, sra			, T0	, ">>>"		,	"dst = lhs >>> rhs + oth"			, "arithmetic shift right"				, 0)				\
	X(ALU,alu,DIV		, div		, 0x09, div			, T0	, "/"		,	"dst = lhs / rhs + oth"				, "arithmetic divide"					, 0)				\
	X(ALU,alu,MUL		, mul		, 0x0a, mul			, T0	, "*"		,	"dst = lhs * rhs + oth"				, "arithmetic multiply"					, 0)				\
	X(ALU,alu,REM		, rem		, 0x0b, rem			, T0	, "%"		,	"dst = lhs & rhs + oth"				, "arithmetic remainder/modulo"			, 0)				\
	X(ALU,alu,MULHI		, mulhi		, 0x0c, mulhi		, T0	, "*"		,	"dst = lhs * rhs + oth"				, "arithmetic multiply high"			, 0)				\
	X(ALU,alu,MULUS		, mulus		, 0x0d, mulus		, T0	, "*"		,	"dst = lhs * rhs + oth"				, "arithmetic multiply unsigned signed"	, 1)				\
	X(ALU,alu,DIVU		, divu		, 0x0e, divu		, T0	, "/"		,	"dst = lhs / rhs + oth"				, "arithemtic divide unsigned"			, 1)				\
	X(ALU,alu,NOT		, not		, 0x0f, not			, T0	, "!"		,	"dst = lhs ! rhs + oth"				, "arithemtic not"						, 0)				\
	X(ALU,alu,CLT		, clt		, 0x10, clt			, T0	, "<"		,	"dst = lhs < rhs + oth"				, "compare less than"					, 0)				\
	X(ALU,alu,CLE		, cle		, 0x11, cle			, T0	, "<="		,	"dst = lhs <= rhs + oth"			, "compare less than equals"			, 0)				\
	X(ALU,alu,CLTU		, cltu		, 0x12, cltu		, T0	, "<="		,	"dst = lhs <= rhs + oth"			, "compare less than unsigned"			, 1)				\
	X(ALU,alu,CEQ		, ceq		, 0x13, ceq			, T0	, "=="		,	"dst = lhs == rhs + oth"			, "compare equals"						, 0)				\
	X(ALU,alu,CNE		, cne		, 0x14, cne			, T0	, "!="		,	"dst = lhs != rhs + oth"			, "compare not equals"					, 0)				\
	X(ALU,alu,CLEU		, cleu		, 0x15, cleu		, T0	, "<="		,	"dst = lhs <= rhs + oth"			, "compare less than equals unsigned"	, 1)				\
	X(ALU,alu,COR		, cor		, 0x16, cor			, T0	, "|"		,	"dst = lhs || rhs + oth"			, "compare or"							, 0)				\
	X(ALU,alu,CAND		, cand		, 0x17, cand		, T0	, "&"		,	"dst = lhs && rhs + oth"			, "compare and"							, 0)				\
	X(ALU,alu,SIEX		, siex		, 0x18, siex		, T0	, "int"		,	"dst = lhs extend rhs + oth"		, "sign extend"							, 0)				\
	X(ALU,alu,CLTMV		, cltmv		, 0x19, cltmv		, T0	, "<?"		,	"dst = lhs <? rhs + oth"			, "compare less than move"				, 0)				\
	X(ALU,alu,CLEMV		, clemv		, 0x1a, clemv		, T0	, "<=?"		,	"dst = lhs <=? rhs + oth"			, "compare less than equals move"		, 0)				\
	X(ALU,alu,CEQMV		, ceqmv		, 0x1b, ceqmv		, T0	, "==?"		,	"dst = lhs ==? rhs + oth"			, "compare compare equals move"			, 0)				\
	X(ALU,alu,CNEMV		, cnemv		, 0x1c, cnemv		, T0	, "!=?"		,	"dst = lhs !=? rhs + oth"			, "compare not equals move"				, 0)				\
	X(ALU,alu,MAX		, max		, 0x1e, max			, T0	, "max"		,	"dst = lhs max rhs + oth"			, "maximum"								, 0)				\
	X(ALU,alu,MIN		, min		, 0x1f, min			, T0	, "min"		,	"dst = lhs min rhs + oth"			, "minimum"								, 0)				\
	X(ALU,alu,POPCNT	, popcnt	, 0x20, popcount	, T0	, "pop"		,	"dst = lhs pop rhs + oth"			, "popcount"							, 0)				\
	X(ALU,alu,RAND		, rand		, 0x21, rand		, T0	, "rand"	,	"dst = lhs rand rhs + oth"			, "random"								, 0)				\







#define WCPU_SUBPATH_MEM_LIST(X)\
	X(MEM,mem,LDI		, ldi		, 0x00, ld			, T0	, "ld"		,	"dst = mem[l1 + l2 + l3]"				, "load"					, 0)				\
	X(MEM,mem,STI		, sti		, 0x01, st			, T0	, "st"		,	"dst = 0, mem[l1] = l2 + l3"			, "store"					, 0)				\
	X(MEM,mem,SP		, sp		, 0x02, sp			, T0	, "sp"		,	"dst = sp, sp = l1 + l2 + l3"			, "get/set sp"				, 0)				\
	X(MEM,mem,PUSH		, push		, 0x03, push		, T0	, "push"	,	"dst = 0, mem[sp+1] = l1 + l2 + l3"		, "push "					, 0)				\
	X(MEM,mem,POP		, pop		, 0x04, pop			, T0	, "pop"		,	"dst = mem[sp-]"						, "pop"						, 0)				\
	X(MEM,mem,INCSP		, incsp		, 0x05, incsp		, T0	, "sp+"		,	"dst = 0, sp+=l1 + l2 + l3"				, "increment sp"			, 0)				\
	X(MEM,mem,DECSP		, descp		, 0x06, decsp		, T0	, "sp-"		,	"dst = 0, sp-=l1 + l2 + l3"				, "decrement sp"			, 0)				\
	X(MEM,mem,SFP		, sfp		, 0x07, sfp			, T0	, "sfp"		,	"dst = sfp, sfp = l1 + l2 + l3"			, "get/set sfp"				, 0)				\
	X(MEM,mem,LDS		, lds		, 0x08, lds			, T0	, "ld[sp]"	,	"dst = mem[sp + l1]"					, "load stack"				, 0)				\
	X(MEM,mem,STS		, sts		, 0x09, sts			, T0	, "st[sp]"	,	"dst = 0, mem[sp + l2 +] = l1"			, "store stack"				, 0)				\
//	X(MEM,mem,LDI		, ldi		, 0x0a, ld			, T0	, "[]"		,	"dst = mem[l1 + l2 + l3]"				, "arithmetic add"						,"dst,lhs,rhs,oth,swap", 0)				\
//	X(MEM,mem,LDI		, ldi		, 0x0b, ld			, T0	, "[]"		,	"dst = mem[l1 + l2 + l3]"				, "arithmetic add"						,"dst,lhs,rhs,oth,swap", 0)				\
//	X(MEM,mem,LDI		, ldi		, 0x0c, ld			, T0	, "[]"		,	"dst = mem[l1 + l2 + l3]"				, "arithmetic add"						,"dst,lhs,rhs,oth,swap", 0)				\
//	X(MEM,mem,LDI		, ldi		, 0x0d, ld			, T0	, "[]"		,	"dst = mem[l1 + l2 + l3]"				, "arithmetic add"						,"dst,lhs,rhs,oth,swap", 0)				\
//	X(MEM,mem,LDI		, ldi		, 0x0e, ld			, T0	, "[]"		,	"dst = mem[l1 + l2 + l3]"				, "arithmetic add"						,"dst,lhs,rhs,oth,swap", 0)				\
//	X(MEM,mem,LDI		, ldi		, 0x0f, ld			, T0	, "[]"		,	"dst = mem[l1 + l2 + l3]"				, "arithmetic add"						,"dst,lhs,rhs,oth,swap", 0)				\



#define WCPU_SUBPATH_JMP_LIST(X)\
	X(JMP,jmp,BEQ		, beq		, 0x00, beq			, T0	, "=="		,	"dst = 0, goto l3 iff l1 == l2"						, "branch equals"								, 0)				\
	X(JMP,jmp,BNE		, bne		, 0x01, bne			, T0	, "!="		,	"dst = 0, goto l3 iff l1 == l2"						, "branch not equals"							, 0)				\
	X(JMP,jmp,JMP		, jmp		, 0x02, jmp			, T0	, ""		,	"dst = 0, goto l3 + l2 + l1"						, "jump"										, 0)				\
	X(JMP,jmp,BLT		, blt		, 0x03, blt			, T0	, "<"		,	"dst = 0, goto l3 iff l1 < l2"						, "branch less than"							, 0)				\
	X(JMP,jmp,BLE		, ble		, 0x04, ble			, T0	, "<="		,	"dst = 0, goto l3 iff l1 <= l2"						, "branch less than equals"						, 0)				\
	X(JMP,jmp,CALL		, call		, 0x05, call		, T0	, "call"	,	"dst = 0, call l1 + l2 + l3 "						, "call"										, 0)				\
	X(JMP,jmp,RET		, ret		, 0x06, ret			, T0	, "ret"		,	"dst = 0, return"									, "return"										, 0)				\
	X(JMP,jmp,BLTU		, bltu		, 0x07, bltu		, T0	, "<"		,	"dst = 0, goto l3 iff (uint)l1 < (uint)l2)"	, "branch less than unsigned"							, 1)				\
	X(JMP,jmp,BLEU		, bleu		, 0x08, bleu		, T0	, "<="		,	"dst = 0, goto l3 iff (uint)l1 <= (uint)l2)", "branch less than equals unsigned"					, 1)				\




//CALLS
//



#define WCPU_SUBPATH_SYS_LIST(X)\
	X(SYS,sys,BREAK			,break		, 0x0f, break		, T0	, "break"	,	"dst = 0, break"										, "break"										, 0)				\
	X(SYS,sys,SET_CD_PTR	,set_cd_ptr	, 0x16, setcd		, T0	, "set_cd"	,	"dst = 0, set cd = l1 + l2 + l3 "						, "set code description"						, 0)				\
	X(SYS,sys,CALL_CD_PTR	,call_cd_ptr, 0x17, callcd		, T0	, "call_cd"	,	"dst = 0, call cd = l1 + l2 + l3 "						, "call code description"						, 0)				\

//SYS

//get timer
#define SYS_TIME 			0x01
//get retired
#define SYS_RETIRED 		0x02
//set, get fault jump
#define SYS_JUMP			0x03
//sys get mode
#define SYS_MODE			0x04




//function table
//user or kernal mode
#define SYS_FUNC_TABLE_PTR	0x05
//gets the function table



//add function to function table
#define SYS_ADD_FUNC			0x09

//cache prediction off
#define SYS_PREFETCH_OFF 		0x0a
//cache prediction on
#define SYS_PREFETCH_ON 		0x0b
//cache fence
#define SYS_FENCE				0x0c
//cache flush
#define SYS_FLUSH				0x0d



//Syscalls
#define SYS_CALL				0x0e
//sysbreak
#define SYS_BREAK				0x0f

//frame description table ptr
#define SYS_CDT_PTR			0x10
//frame description table len

//permisions TODO
//set permisions
#define SYS_PERMS			0x14
//get permisions

//furhter shit needed
//system set code description ptr
#define SYS_SET_CD_PTR			0x16
//call to code description ptr
#define SYS_CALL_CD_PTR			0x17
//return code description ptr
#define SYS_RET_CD_PTR			0x18
//poke at the user reg file
#define SYS_POKE_USER			0x19
//press on the user register file
#define SYS_PRESS_USER			0x1a

//system interupt usermode timer
#define SYS_INTERUPT_TIMER 	0x1b


//these are the realloc ptr's that set dest address as hashes resolved by the vpu
#define SYS_REL_BIND_PTR		0x1c
#define SYS_REL_UNBIND_PTR		0x1d
#define SYS_REL_QUERY_PTR		0x1e
#define SYS_REL_CHECK_PTR		0x1f

#define SYS_HALT				0x20

#define SYS_STRIKE				0x21
#define SYS_ID					0x22
//FPU
//add
//(lane1 + lane2)
//packed 2 times
//
#define FPU_FADD				0x00
//subtract
//(lane1 - lane2)

//packed
#define FPU_FSUB				0x01

//multiply add (lane1 * lane2 + lane3)
#define FPU_FMADD				0x02

//multiply sub (lane1 * lane2 - lane3)
#define FPU_FMSUB				0x03

//multiply	(lane1 * lane2)
#define FPU_FMUL				0x04

//deiv
#define FPU_FDIV				0x05

//sqrt
#define FPU_FSQRT				0x06

//sign injection
#define FPU_FSGNJ				0x07

//sign neg injection
#define FPU_FSGNJN				0x08

//sign xor injection
#define FPU_FSGNJX				0x09

//min
#define FPU_FMIN				0x0a
//max
#define FPU_FMAX				0x0b
//convert to signed int
#define FPU_FCVI				0x0c

//convert to unsigned int
#define FPU_FCVU				0x0d

//convert from int
#define FPU_ICVF				0x0e

//convert from unsigned int
#define FPU_UCVF				0x0f

//move float to interger file
#define FPU_FMVI				0x10
//move int to float file
#define FPU_IMVF				0x11
//float equal
#define FPU_FEQ					0x12
//float less than
#define FPU_FLT					0x13
//float less than equal
#define FPU_FLE					0x14
//float cclassify
#define FPU_FCLASS				0x15
//float cosine
#define FPU_FCOS				0x17

#define FPU_FSIN				0x18

//double add
#define FPU_DADD				0x19

#define FPU_DSUB				0x20

#define FPU_DMADD				0x21

//multiply sub (lane1 * lane2 - lane3)
#define FPU_DMSUB				0x22

//multiply	(lane1 * lane2)
#define FPU_DMUL				0x23

//deiv
#define FPU_DDIV				0x24

//sqrt
#define FPU_DSQRT				0x25

//sign injection
#define FPU_DSGNJ				0x26

//sign neg injection
#define FPU_DSGNJN				0x27

//sign xor injection
#define FPU_DSGNJX				0x28

//min
#define FPU_DMIN				0x29
//max
#define FPU_DMAX				0x2a
//convert to signed int
#define FPU_DCVI				0x2b

//convert to unsigned int
#define FPU_DCVU				0x2c

//convert from int
#define FPU_DCVF				0x2d

//convert from unsigned int
#define FPU_UCVD				0x2e

//move float to interger file
#define FPU_DMVI				0x2f
//move int to float file
#define FPU_IMVD				0x30
//float equal
#define FPU_DEQ					0x31
//float less than
#define FPU_DLT					0x32
//float less than equal
#define FPU_DLE					0x33
//float cclassify
#define FPU_DCLASS				0x34
//float cosine
#define FPU_DCOS				0x35

#define FPU_DSIN				0x36


//TODO

//ATOMIC

//TODO



//REVIEW

/*	currently this is bare bones with further additions added later for all the needed systems.
*	register 0 is hardwired to 0 because it makes the architecture significantly more capable of being masked
*
*	future paths may include
*
*	1:MACRO
*		Marco path for interaction with specific instructions for the cache prefetcher to use
*
*		may include:
*			MACRO_TRAVEL_LL
*			MACRO_MAP
*			MACRO_STRLEN //secured
*			MACRO_STRCPY //secured
*			MACRO_STRCHR //secured
*			MACRO_STRCMP //secured
*			MACRO_MEMCPY
*			MACRO_MEMSET
*			MACRO_MEMMOV
*			MACRO_MEMCHR
			MACRO_MEMCLR
*			MACRO_BSEARCH_GT
*			MACRO_BSEARCH_GE
*
*
*	2:SEC/CRYPT
*		Security/Cyptography path
*we
*		may include:
*			SEC_RAND
*			SEC_SRAND
*
*
*	3:SIMD
*		Single instruction multiple data path
*
*		LIKELY NEW REG FILE
*
*		will be big and wide.
*
*		may include:
*
*			SIMD_SD
*			SIMD_LD
*			SIMD_IND_TO_REG
*			SIMD_REG_TO_IND
*
*			integer
*			set modes of integer (unsigned, 8 bit, 16 bit, 32 bit, 64 bit or whatever
*			SIMD_I_MODE
*			dot product
*			SIMD_I_DOT
*			add together
*			SIMD_I_ADD
*			subtract
*			SIMD_I_SUB
*			multiply
*			SIMD_I_MUL
*			SIMD_I_DIV
*			SIMD_I_REM
*			root of all the elements
*			SIMD_I_ROOT
*			compare somehow
*			SIMD_I_CMP
*
*			sum of all slices
*			SIMD_I_SUM
*			product of all sliced
*			SIMD_I_PRODUCT
*			floor
*			SIMD_I_FLOOR
*			ceiling
*			SIMD_I_CEIL
*			get lowest
*			SIMD_I_LOW
*			get highest
*			SIMD_I_HIG
*
* 			SIMD_I_ROT_L
* 			SIMD_I_ROT_R
*
*			convert to floating point
*			SIMD_I_TO_F
*
*
*			floating point
*			SIMD_F_MODE
*			dot product
*			SIMD_F_DOT
*			subrract
*			SIMD_F_ADD
*			add and subtract floats
*			SIMD_F_SUB
*			multiply floats
*			SIMD_F_MUL
*			divide floats
*			SIMD_F_DIV
*
*			SIMD_F_SIN
*			SIMD_F_COS
*			SIMD_F_TAN
*
*			SIMD_F_SINH
*			SIMD_F_COSH
*			SIMD_F_TANH
*
*			SIMD_F_ISIN
*			SIMD_F_ICOS
*			SIMD_F_ITAN
*			SIMD_F_ISINH
*			SIMD_F_ICOSH
*			SIMD_F_ITANH
*
*			SIMD_F_RECP
*			SIMD_F_SQRT
*
*			SIMD_F_LN
*			SIMD_F_LOG
*			SIMD_F_EXP
*
*			convert to integer
*			SIMD_F_TO_I
*	4:NET
*		Networking path for interaction over the internet
*	5:HRD
*		Hardware path for interaction over hardware,
*	6:STR
*		String path for 8 bit addressing simply
*
*/

#define CODE_DESC_STRIDE 16
#define CACHE_LINE_SIZE 8

#endif
