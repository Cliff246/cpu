#include "flags.h"
#include "decoder.h"
#include "eerror.h"
#include "commons.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strtools.h"
#include "parser.h"
#include <stdbool.h>




int get_register(char *keyword)
{
	const char *const reg_mnemonics[] = {
    	"zero", "x0", "null",
    	"x1", "a0",
    	"x2", "a1",
    	"x3", "a2",
    	"x4", "a3",
    	"x5", "t0", "ta",
    	"x6", "t1", "tb",
    	"x7", "t2", "tc",
    	"x8", "t3", "td",
    	"x9", "t4", "te",
    	"x10", "t5", "tf",
    	"x11", "t6", "tg",
    	"x12", "t7", "th",
    	"x13", "t8", "ti",
    	"x14", "t9", "tj",
    	"x15", "t10", "tk",   // ✅ added comma here
    	"x16", "g0",
    	"x17", "g1",
    	"x18", "g2",
    	"x19", "g3",
    	"x20", "g4",
    	"x21", "g5",
    	"x22", "g6",
    	"x23", "g7",
    	"x24", "s0",
    	"x25", "s1",
    	"x26", "s2",
    	"x27", "s3",
    	"x28", "s4",
    	"x29", "sys",
    	"x30", "aux0",
    	"x31", "aux1",
    	"x32", "a4",
		"x33", "a5",
		"x34", "a6",
		"x35", "a7",
		"x36", "a8",
		"x37", "a9",
		"x38", "s5",
		"x39", "s6",
		"x40", "s7",
		"x41", "s8",
		"x42", "s9",
		"x43", "s10", "exs0",
		"x44", "s11", "exs1",
		"x45", "s12", "exs2",
		"x46", "s13", "exs3",
		"x47", "s14", "exs4",
    	"x48", "t11", "tx", "ext0",
		"x49", "t12", "ty", "ext1",
		"x50", "t13", "tz", "ext2",
		"x51", "t14", "tu", "ext3",
		"x52", "six", "s15",
		"x53", "siy", "s16",
		"x54", "siz", "s17",
		"x55", "tix", "t15",
		"x56", "tiy", "t16",
		"x57", "tiz", "t17",
		"x58", "t18",
		"x59", "t19",
		"x60", "t20",
		"x61", "t21",
		"x62", "t22",
		"x63", "acc",
	};

	int regvalue[] = {
    	0, 0, 0,
    	1, 1,
    	2, 2,
    	3, 3,
    	4, 4,
    	5, 5, 5,
    	6, 6, 6,
    	7, 7, 7,
    	8, 8, 8,
    	9, 9, 9,
    	10, 10, 10,
    	11, 11, 11,
    	12, 12, 12,
    	13, 13, 13,
    	14, 14, 14,
    	15, 15, 15,
    	16, 16,
    	17, 17,
    	18, 18,
    	19, 19,
    	20, 20,
    	21, 21,
    	22, 22,
    	23, 23,
    	24, 24,
    	25, 25,
    	26, 26,
    	27, 27,
    	28, 28,
    	29, 29,
    	30, 30,
    	31, 31,
    	32, 32,
		33, 33,
		34, 34,
		35, 35,
		36, 36,
		37, 37,
		38, 38,
		39, 39,
   		40, 40,
		41, 41,
		42, 42,
		43, 43, 43,
		44, 44, 44,
		45, 45, 45,
		46, 46, 46,
		47, 47, 47,
    	48, 48, 48, 48,
		49, 49, 49, 49,
		50, 50, 50, 50,
		51, 51, 51, 51,
		52, 52, 52,
		53, 53, 53,
		54, 54, 54,
 		55, 55, 55,
		56, 56, 56,
		57, 57, 57,
		58, 58,
		59, 59,
		60, 60,
		61, 61,
		62, 62,
		63, 63
	};


	int code = determine_code(keyword, reg_mnemonics, ARYSIZE(reg_mnemonics));
	if(code != -1)
	{

		return regvalue[code];
	}
	else
	{
		return -1;
	}
}



int get_alu_subpath(char *keyword)
{


	const char *const alu_mnemonics[] = {
		"add",
		"sub",
		"subu",
		"and",
		"or",
		"xor",
		"sll",
		"srl",
		"sra",
		"div",
		"mul",
		"rem",
		"mulhi",
		"mulu",
		"mulus",
		"divu",
		"not",
		"cle",
		"clt",
		"cltu",
		"cne",
		"ceq",
	};

	const int opvalue[] = {
		ALU_ADD,
		ALU_SUB,
		ALU_SUBU,
		ALU_AND,
		ALU_OR,
		ALU_XOR,
		ALU_SLL,
		ALU_SRL,
		ALU_SRA,
		ALU_DIV,
		ALU_MUL,
		ALU_REM,
		ALU_MULHI,
		ALU_MULU,
		ALU_MULUS,
		ALU_DIVU,
		ALU_NOT,
		ALU_CLE,
		ALU_CLT,
		ALU_CLTU,
		ALU_CNE,
		ALU_CEQ,
	};

	int code = determine_code(keyword, alu_mnemonics, ARYSIZE(alu_mnemonics));
	//printf("keyword %s %d\n", keyword, code);

	if(code != -1)
	{

		return opvalue[code];
	}
	else
	{
		return -1;
	}


}
int get_mem_subpath(char *keyword)
{
	const char *const mem_mnemonics[] = {
		"ld",
		"st",
		"push",
		"pop",
		"sp",
		"sfp",
		"lds",
		"sts",
		"isp",		//increment stack ptr
		"dsp",		//decrement stack ptr
		"ldf",
		"stf"
	};

	const int opvalue[] = {
		MEM_LDI,
		MEM_STI,
		MEM_PUSH,
		MEM_POP,
		MEM_SP,
		MEM_SFP,
		MEM_LDS,
		MEM_STS,
		MEM_INCSP,
		MEM_DECSP,
		MEM_LDF,
		MEM_STF,
	};


	int code = determine_code(keyword, mem_mnemonics, ARYSIZE(mem_mnemonics));

	if(code != -1)
	{

		return opvalue[code];
	}
	else
	{
		return -1;
	}
}
int get_jmp_subpath(char *keyword)
{
	const char *const jmp_mnemonics[] = {
		"jmp",
		"bne",
		"blt",
		"beq",
		"ble",
		"call",
		"ret",
		"bleu",
		"bltu",
	};

	const int opvalue[] = {
		JMP_JMP,
		JMP_BNE,
		JMP_BLT,
		JMP_BEQ,
		JMP_BLE,
		JMP_CALL,
		JMP_RET,
		JMP_BLEU,
		JMP_BLTU,
	};

	int code = determine_code(keyword, jmp_mnemonics, ARYSIZE(jmp_mnemonics));
	if(code != -1)
	{

		return opvalue[code];
	}
	else
	{
		return -1;
	}
}

int get_sys_subpath(char *keyword)
{

	const char *const sys_mnemonics[] = {
		"call",
		"dcall",
		"dset",
		"break",
	};

	const int opvalue[] = {
		SYS_CALL,
		SYS_CALL_CD_PTR,
		SYS_SET_CD_PTR,
		SYS_BREAK,
	};
	int code = determine_code(keyword, sys_mnemonics, ARYSIZE(sys_mnemonics));
	if(code != -1)
	{

		return opvalue[code];
	}
	else
	{
		return -1;
	}

}



int get_path(char *keyword)
{


	const char *const pathwords[] = {
		"alu",
		"mem",
		"jmp",
		"sys",
	};

	const int pathvalue[] = {
		PATH_ALU,
		PATH_MEM,
		PATH_JMP,
		PATH_SYS,

	};
	int code = determine_code(keyword, pathwords, ARYSIZE(pathwords));
	if(code == -1)
	{
		printf("not a path code\n");
		return -1;
	}
	return pathvalue[code];
}




int get_subpath(int path, char *keyword)
{
	//printf("path: %d\n", path);
	switch(path)
	{

		case PATH_ALU:
			return get_alu_subpath(keyword);
			break;
		case PATH_JMP:
			return get_jmp_subpath(keyword);
			break;
		case PATH_MEM:
			return get_mem_subpath(keyword);
			break;
		case PATH_SYS:
			return get_sys_subpath(keyword);
			break;
		default:
			return -1;
	}
}


bool valid_reference(char **str, int length)
{
	//
	if(str == NULL)
		return false;
	if(length < 0)
		return false;


	return true;
}
bool valid_instruction(char **str, int length)
{
	if(str == NULL)
		return false;
	if(length < 0)
		return false;





	return true;
}
