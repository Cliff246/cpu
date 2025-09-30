#include "flags.h"
#include "decoder.h"

#include "commons.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asm_str_stuff.h"

#define ARYSIZE(ary) sizeof(ary)/sizeof(*ary)


void print_inst(inst_t *inst)
{
	printf("path:%d subpath %d: rd: %d, rs1: %d, rs2: %d, aux: %d, immf: %d\n", inst->path, inst->subpath, inst->rd, inst->rs1, inst->rs2, inst->aux, inst->immflag);
}

inst_t decode_inst(int32_t instr)
{
	inst_t in;
	in.path = (instr >> 28) & 0xF;
	in.subpath = (instr >> 21) & 0x7F;
	in.rd = (instr >> 16) & 0x1F;
	in.rs1 = (instr >> 11) & 0x1F;
	in.rs2 = (instr >> 6) & 0x1F;
	in.aux = (instr >> 2) & 0xf;
	in.immflag = instr & 0x3;
	return in;
}

uint32_t encode_inst(inst_t *inst)
{

	return ((inst->path << 28) | (inst->subpath << 21) | (inst->rd << 16) | (inst->rs1 << 11) | (inst->rs2 << 6) | (inst->aux << 2 & 0x7) | inst->immflag);
}

uint64_t encode(uint64_t path, uint64_t subpath, uint64_t rd, uint64_t rs1, uint64_t rs2, uint64_t aux, uint64_t immf)
{
	uint64_t inst = ( (path & 0xF) << 28) | ((subpath &0x7f) << 21) | ((rd &0x1f) << 16) | ((rs1 &0x1f) << 11) | ((rs2 &0x1f) << 6) | ((aux & 0xf) << 2) | (immf);
	print_bin(inst, 32, 1);
	return inst;
}


void invalid_inst(char **splits, int length, inst_t *inst)
{

}

void inst_no_imm(char **splits, int length, inst_t *inst)
{
	int path = get_path(splits[0]);	
	if(path == -1)
	{
		inst->err = not_valid;
		return;
	}

	//printf("path %d\n", path);
	inst->path = path;
	int subpath = get_subpath(path, splits[1]);	
	inst->subpath = subpath; 

	int rd = get_register(splits[2]);
	int rs1 = get_register(splits[3]);
	int rs2 = get_register(splits[4]);
	inst->rd =rd;
	inst->rs1= rs1;
	inst->rs2= rs2;
	inst->aux = 0;

	inst->err = valid;
	inst->imm = 0;
	inst->immflag = 0;	
	inst->immref = NULL;
}

void inst_imm(char **splits, int length, inst_t *inst)
{
	int path = get_path(splits[0]);	
	inst->path = path; 
	if(path == -1)
	{
		inst->err = not_valid;
		return;
	}

	int subpath = get_subpath(inst->path,splits[1]);	
	inst->subpath = subpath; 

	int rd = get_register(splits[2]);
	int rs1 = get_register(splits[3]);
	int rs2 = get_register(splits[4]);

	inst->rd =rd;
	inst->rs1= rs1;
	inst->rs2= rs2;

	int type = get_number_type(splits[5]);
	uint64_t imm = 0; 
	if(type == 1 || type == 2)
	{
		imm = atoi(splits[5]);
	}
	if(type == 2)
	{
		imm = convert_to_hex(splits[5]);	
	}
	if(type == 3)
	{
		imm = convert_to_oct(splits[5]);
	}

	if(type == 0)
	{
		if(valid_name(splits[5]))
		{
			char *dup = strdup(splits[5]);
			inst->immref = dup; 
		}
		else
		{
			inst->err = not_valid;
	
		}
	}


	inst->imm = imm; 
	inst->immflag = 1;	
	inst->err = valid;
}





inst_t create_instruction(char *line, int linen)
{

	//printf("line: %s\n", line);
	char **splits = NULL; 
	int splits_len = split_str(line, &splits, " ,.\t"); 

	inst_t inst;
	//clear this
	memset(&inst, 0, sizeof(inst_t));


	inst.line = linen;
	inst.linestr = strdup(line);
	


	if(splits_len != 6 && splits_len != 5)
	{
		invalid_inst(splits, splits_len, &inst);	
	}	
	else if(splits_len == 5)
	{
		inst_no_imm(splits, splits_len, &inst);
	}
	else if(splits_len == 6)
	{
		inst_imm(splits, splits_len, &inst);
	}
	free(splits);
	return inst;
}


int get_register(char *keyword)
{
	const char *const reg_mnemonics[] = {
		"zero",
		"x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9", "x10",
		"x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "x19", "x20",
		"x21", "x22", "x23", "x24", "x25", "x26", "x27", "x28", "x29", "x30",
		"x31"
	};

	int regvalue[] = {
		0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
		20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
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
		"ceq"
	};

	const int opvalue[] = {
		ALU_ADD,
		ALU_SUB,
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
		"get_sp",
		"set_sp",
		"get_sfp",
		"set_sfp"
	};

	const int opvalue[] = {
		MEM_LD,
		MEM_SD,
		MEM_PUSH,
		MEM_POP,
		MEM_GET_SP,
		MEM_SET_SP,
		MEM_GET_SFP,
		MEM_GET_SFP


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
		"ble",
		"beq",
		"call",
		"ret",
	};

	const int opvalue[] = {
		JP_JMP,
		JP_BNE,
		JP_BLT,
		JP_BEQ,
		JP_CALL,
		JP_RET	
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
		"syscall"
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
