#ifndef __FLAGS__
#define __FLAGS__


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





//instruction convention is always
//4			7		5		5		5	 3		2
//[31:28] [27:21] [20:16] [15:11] [10:6] [5:2] [1:0]
//path    subpath  rd      rs1     rs2   aux    immf

#define STR(x) #x

//immf 00 = NONE
#define IMMF_EMPTY 		0

//immf 01 = QUARTER DWORD
#define IMMF_QUARTER	1
//immf 10 = HALF DWORD
#define IMMF_HALF		2
//immf 11 = FULL PULL
#define IMMF_FULL		3


//AUX FLAGS
//
#define AUX_ZERO		0
#define AUX_RELATIVE	1
#define AUX_

//ALL internal operations that are get and set are done via rd and rs1.

// as in get is just setting rd,
// as in set is just setting rs1

//so rd = x0, rs1 = x0 = nop
//so rd != x0, rs1 = x0 = get
//so rd = x0, rs1 != x0 = set
//so rd != x0, rs1 != x0 = exchange


//rd = rs1 + rs2 + imm
#define ALU_ADD 	0x00
//rd = rs1 - rs2 - imm
#define ALU_SUB 	0x01
//rd = rs1 & (immflag)? rs2 : imm
#define ALU_AND		0x02
//rd = rs1 | (immflag)? rs2 : imm
#define ALU_OR		0x03
//rd = rs1 ^ (immflag)? rs2 : imm
#define ALU_XOR 	0x04
//shift left logical
#define ALU_SLL 	0x05
//shift right logical
#define ALU_SRL		0x06
//shift right arithmetic
#define ALU_SRA 	0x07
//divide signed
#define ALU_DIV 	0x08
//multiply signed low
#define ALU_MUL 	0x09
//remainder
#define ALU_REM 	0x0a
//multiply unsigned
#define ALU_MULHI	0x0c
//multiply unsigned
#define ALU_MULU	0x0d
//multiply unsigned signed
#define ALU_MULUS	0x0e
#define ALU_DIVU 	0x20
//not
#define ALU_NOT 	0x0f
//compare less than
#define ALU_CLT		0x10
//compare less than equals
#define ALU_CLE		0x11
//compare less than unsigned
#define ALU_CLTU	0x12
//compare equals
#define ALU_CEQ		0x13
//compare not equals
#define ALU_CNE		0x14





//MEMORY
//Memory load
#define MEM_LD 		0x00
//Memory store
#define MEM_SD		0x01
//MEMORY get stack ptr, set stack ptr
#define MEM_SP	0x02
//Memory Push
#define MEM_PUSH	0x04
//Memory pop
#define MEM_POP		0x05
//memory get stack frame ptr, get stack frame
#define MEM_SFP	0x06


//branch equal
#define JP_BEQ 		0x01
//branch not equal
#define JP_BNE		0x02
//jump jmp
#define JP_JMP		0x03
//jmp branch less than
#define JP_BLT		0x04
//jmp branch less than equal
#define JP_BLE		0x05

//CALLS
//call function
//
#define JP_CALL		0x06
//return from function
#define JP_RET		0x07




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

//FPU

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
*
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

#define CODE_DESC_STRIDE 128

#endif
