#include "flags.h"
#include "decoder.h"

#include "commons.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARYSIZE(ary) sizeof(ary)/sizeof(*ary)

inst_t decode_inst(int32_t instr)
{
	inst_t in;
	in.path = (instr >> 28) & 0xF;
	in.subpath = (instr >> 20) & 0xFF;
	in.rd = (instr >> 15) & 0x1F;
	in.rs1 = (instr >> 10) & 0x1F;
	in.rs2 = (instr >> 5) & 0x1F;
	in.aux = (instr >> 2) & 0x7;
	in.immflag = instr & 0x3;
	return in;
}

int32_t encode_inst(inst_t *inst)
{

	return ((inst->path << 28) | (inst->subpath << 20) | (inst->rd << 15) | (inst->rs1 << 10) | (inst->rs2 << 5) | (inst->aux << 2) | inst->immflag);

}

uint64_t encode(uint64_t path, uint64_t subpath, uint64_t rd, uint64_t rs1, uint64_t rs2, uint64_t aux, uint64_t immf)
{
	uint64_t inst = ( (path & 0xF) << 28) | ((subpath &0xff) << 20) | ((rd &0x1f) << 15) | ((rs1 &0x1f) << 10) | ((rs2 &0x1f) << 5) | ((aux & 0x8) << 2) | (immf);
	print_bin(inst, 32, 1);
	return inst;
}




int determine_code(char *keyword, const char *const mnemonics[], int length)
{
	

	int *keycount = calloc(length, sizeof(int));
	if(keycount == 0)
	{
		printf("could not allocate enough memory line: %d, file %s \n", __LINE__, __FILE__);
		exit(1);
	}

	
	int i = 0;
	for(char *ch = keyword; *ch; ++ch, ++i)
	{
		
		for(int pool = 0; pool < length; ++pool)
		{
			if(keyword[pool] == -1)
			{
				continue;
			}
			if(mnemonics[pool][i] == '\0')
			{
				keycount[pool] = -1;
			}
			else if(mnemonics[pool][i] == *ch)
				keycount[pool] += 1;
			else
				keycount[pool] = -1;
		}	
	}	
	
	int max = 0, index = 0;
	for(int k = 0; k < length; ++k)
	{
		if(keycount[k] > max)
		{
			max = keycount[k];
			index = k;
		}
	}
	
	free(keycount);

	if(strlen(mnemonics[index]) == max)
	{
		return index;
	}	
	return -1;
}


char *clear_whitespace(char *str)
{
	int length = strlen(str);
	char temp[length + 1];
	memset(temp, 0, length + 1);
	int count = 0;
	for(char *ch = str; *ch; ch++)
	{
		if(*ch != ' ')
		{
			temp[count++] = *ch;
		}
	}
	return strdup(temp);	

}


int split_str(const char *string, char ***ptr, const char *delims)
{
	
	size_t slength = strlen(string);
	size_t dlength = strlen(delims);
	char *where = (char *)malloc(sizeof(char) * slength);
	memset(where, 0, slength * sizeof(int)); 
	int splits = 0;
	
	printf("%d\n", slength);
	bool  wasspliting = false, didsplit;
	int last = 0;
	for(int x = 0; x < slength; ++x)
	{
		
		didsplit = false;
		for(int y = 0; y < dlength; ++y)
		{
			if(string[x] == delims[y])
			{	
				didsplit = true;
				where[x] = string[x];
				continue;
			}	
		}
		if(didsplit)
		{
			if(wasspliting == false)
			{
				wasspliting = true;
			}
		}
		else
		{
			//did not split
			if(wasspliting == true)
			{
				splits++;
				wasspliting = false;
			}
		}
		
	}

	for(int i = 0; i < slength; ++i)
	{

		printf("%c %d\n", string[i],where[i]);
	}
	char **splitary = calloc(splits, sizeof(char *));
	char *buffer = malloc(slength + 1);	
	int ibuf = 0;
	int last_good = 0;
	bool back_to_back = false;
	int current = 0;
	for(int i = 0, cur; i < slength; ++i, ++cur)
	{
		if(where[i] == 0)
		{
			if(back_to_back == true)
			{
				last_good = i;
				back_to_back = false;
			}
			buffer[ibuf++] = string[i];
		}			
		else
		{
			if(!back_to_back)
			{
				int delta = (i - 1) - last_good;
				splitary[current++] = strdup(buffer);
				printf("%s\n", splitary[current - 1]);
				ibuf = 0;
				memset(buffer, 0, slength + 1);
				back_to_back = true;
			}
		}
	}
	if(where[slength] == 0)
	{
		int delta = (slength - 1) - last_good;
		splitary[current++] = strdup(buffer);
		printf("%s\n", splitary[current - 1]);
		ibuf = 0;
		memset(buffer, 0, slength + 1);
	}
	else
	{
		int delta = (slength - 2) - last_good;
		splitary[current++] = strdup(buffer);
		printf("%s\n", splitary[current - 1]);
		ibuf = 0;
		memset(buffer, 0, slength + 1);

	}
	free(buffer);
	free(where);
	*ptr = splitary;
	return splits;
}


inst_t create_instruction(char *line, int linen)
{

	int size = strlen(line);
	char *buffer = (malloc(size + 1));
	strcpy(buffer, line);	

	//path, subpath, rd, rs1, rs2, imm	
	char *settings[6];

	char **splits = NULL; 
	printf("he\n");
	int splits_len = split_str(line, &splits, " ,."); 

	inst_t inst;
	inst.line = linen;
	if(splits_len != 6 || splits_len != 5)
	{

		inst.err = not_valid ;
		
	}	
	else if(splits_len == 5)
	{
		int path = get_path(splits[0]);	
		inst.path = path; 
		if(path == -1)
		{
			inst.err = not_valid;
			return inst;
		}
		
		int subpath = get_subpath(inst.path,splits[1]);	
		inst.subpath = subpath; 

		int rd = get_register(splits[2]);
		int rs1 = get_register(splits[3]);
		int rs2 = get_register(splits[4]);

		inst.rd =rd;
		inst.rs1= rs1;
		inst.rs2= rs2;

		inst.err = valid;
		inst.imm = 0;
		inst.immflag = 0;	
		
	}
	else if(splits_len == 6)
	{
		int path = get_path(splits[0]);	
		inst.path = path; 
		if(path == -1)
		{
			inst.err = not_valid;
			return inst;
		}
		
		int subpath = get_subpath(inst.path,splits[1]);	
		inst.subpath = subpath; 

		int rd = get_register(splits[2]);
		int rs1 = get_register(splits[3]);
		int rs2 = get_register(splits[4]);

		inst.rd =rd;
		inst.rs1= rs1;
		inst.rs2= rs2;
		uint64_t imm = atoi(splits[4]);

		inst.imm = imm; 
		inst.immflag = 1;	
		inst.err = valid;

	}
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
		printf("not a code %s\n", keyword);
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
		printf("not a code %s\n", keyword);
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
		"sys",
		"mem",
		"jmp"	
	};

	int code = determine_code(keyword, pathwords, ARYSIZE(pathwords));
	if(code == -1)
	{
		printf("not a code\n");
		return -1;
	}
	if(keyword[0] == '\0')
	{
		return 0;
	}	
	return code;
}




int get_subpath(int path, char *keyword)
{
	switch(path)
	{
		case PATH_ALU:
			return get_alu_subpath(keyword);
		case PATH_JMP:
			return get_jmp_subpath(keyword);
		case PATH_MEM:
			return get_mem_subpath(keyword);
		case PATH_SYS:
			return get_sys_subpath(keyword);
		default:
			return -1;
	}
}
