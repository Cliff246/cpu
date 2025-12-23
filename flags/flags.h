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


//rd = rs1 + rs2 + imm
#define ALU_ADD 	0x00
//rd = rs1 - rs2 - imm
//signed alu add
#define ALU_SUB 	0x01
//rd =
#define ALU_SUBU 	0x02

//rd = rs1 & (immflag)? rs2 : imm
#define ALU_AND		0x03
//rd = rs1 | (immflag)? rs2 : imm
#define ALU_OR		0x04
//rd = rs1 ^ (immflag)? rs2 : imm
#define ALU_XOR 	0x05
//shift left logical
#define ALU_SLL 	0x06
//shift right logical
#define ALU_SRL		0x07
//shift right arithmetic
#define ALU_SRA 	0x08
//divide signed
#define ALU_DIV 	0x09
//multiply signed low
#define ALU_MUL 	0x0a
//remainder
#define ALU_REM 	0x0b
//multiply unsigned
#define ALU_MULHI	0x0c
//multiply unsigned
#define ALU_MULU	0x0d
//multiply unsigned signed
#define ALU_MULUS	0x0e
#define ALU_DIVU 	0x0f
//not
#define ALU_NOT 	0x10
//compare less than
#define ALU_CLT		0x11
//compare less than equals
#define ALU_CLE		0x12
//compare less than unsigned
#define ALU_CLTU	0x13
//compare equals
#define ALU_CEQ		0x14
//compare not equals
#define ALU_CNE		0x15
//compare cle
#define ALU_CLEU 	0x16
//set byte
#define ALU_COR		0x17

#define ALU_CAND	0x18
//sets byte



//sign exten
#define ALU_SIEX	0x1f

//get rd from rs2

//less than move
//dest = (lane2 < lane3)? lane1: 0
#define ALU_LTMV	0x20
//dest = (lane2 <= lane3)? lane1 : 0
#define ALU_LEMV	0x21
//dest = (lane2 == lane3)? lane1 : 0
#define ALU_EQMV	0x22
//dest = (lane2 != lane3)? lane1 : 0
#define ALU_NEMV	0x23


//useful
//dest = max(lane1, (bin)? lane2: lane3)
#define ALU_MAX		0x28
//dest = min(lane1, (bin)? lane2: lane3)
#define ALU_MIN		0x29

#define ALU_POPCNT	0x2a

#define ALU_RAND	0x2b



//MEMORY
//Memory load
#define MEM_LDI 	0x00
//Memory store
#define MEM_STI		0x01
//MEMORY get stack ptr, set stack ptr
#define MEM_SP	0x02
//Memory Push
#define MEM_PUSH	0x04
//Memory pop
#define MEM_POP		0x05
//
#define MEM_INCSP 0x0a
//
#define MEM_DECSP 0x09
//memory get stack frame ptr, get stack frame
#define MEM_SFP	0x06
//memory ld at stack offset
#define MEM_LDS	0x07
//memory store at stack offset
#define MEM_STS 0x08

//memory load float
#define MEM_LDF	0x09
//memory store float
#define MEM_STF	0x0a



//branch equal
#define JMP_BEQ 		0x01
//branch not equal
#define JMP_BNE		0x02
//jump jmp
#define JMP_JMP		0x03
//jmp branch less than
#define JMP_BLT		0x04
//jmp branch less than equal
#define JMP_BLE		0x05




//CALLS
//

//call function
//
#define JMP_CALL		0x06
//return from function
#define JMP_RET		0x07

#define JMP_BLTU		0x08
#define JMP_BLEU		0x09


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

#define SYS_HALT				0xa0

#define SYS_STRIKE				0xa2

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

#endif
