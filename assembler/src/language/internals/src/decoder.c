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
    "x5", "t0",
    "x6", "t1",
    "x7", "t2",
    "x8", "t3",
    "x9", "t4",
    "x10", "t5",
    "x11", "t6",
    "x12", "t7",
    "x13", "t8",
    "x14", "t9",
    "x15", "t10",   // ✅ added comma here
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
    "x32", "x33", "x34", "x35", "x36", "x37", "x38", "x39",
    "x40", "x41", "x42", "x43", "x44", "x45", "x46", "x47",
    "x48", "x49", "x50", "x51", "x52", "x53", "x54", "x55",
    "x56", "x57", "x58", "x59", "x60", "x61", "x62", "x63"
};

int regvalue[] = {
    0, 0, 0,
    1, 1,
    2, 2,
    3, 3,
    4, 4,
    5, 5,
    6, 6,
    7, 7,
    8, 8,
    9, 9,
    10, 10,
    11, 11,
    12, 12,
    13, 13,
    14, 14,
    15, 15,
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
    32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47,
    48, 49, 50, 51, 52, 53, 54, 55,
    56, 57, 58, 59, 60, 61, 62, 63
};


	int code = determine_code(keyword, reg_mnemonics, ARYSIZE(reg_mnemonics));
	if(code != -1)
	{

		return regvalue[code];
	}
	else
	{
		printf("not a valid register code %s\n", keyword);
		exit(1);
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
		printf("not a valid alu code %s\n", keyword);
		exit(1);
	}


}
int get_mem_subpath(char *keyword)
{
	const char *const mem_mnemonics[] = {
		"ld",
		"sd",
		"push",
		"pop",
		"sp",
		"sfp",
	};

	const int opvalue[] = {
		MEM_LD,
		MEM_SD,
		MEM_PUSH,
		MEM_POP,
		MEM_SP,
		MEM_SFP,


	};


	int code = determine_code(keyword, mem_mnemonics, ARYSIZE(mem_mnemonics));

	if(code != -1)
	{

		return opvalue[code];
	}
	else
	{
		printf("not a code %s\n", keyword);
		exit(1);
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
		JP_JMP,
		JP_BNE,
		JP_BLT,
		JP_BEQ,
		JP_BLE,
		JP_CALL,
		JP_RET,
		JP_BLEU,
		JP_BLTU,
	};

	int code = determine_code(keyword, jmp_mnemonics, ARYSIZE(jmp_mnemonics));
	if(code != -1)
	{

		return opvalue[code];
	}
	else
	{
		printf("not a code %s\n", keyword);
		exit(1);
	}
}

int get_sys_subpath(char *keyword)
{

	const char *const sys_mnemonics[] = {
		"call"
	};

	const int opvalue[] = {
		SYS_CALL
	};
	int code = determine_code(keyword, sys_mnemonics, ARYSIZE(sys_mnemonics));
	if(code != -1)
	{

		return opvalue[code];
	}
	else
	{
		printf("not a code %s\n", keyword);
		exit(1);
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
