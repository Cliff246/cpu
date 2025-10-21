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


void print_inst(inst_t *inst)
{
	printf("path:%d subpath %d: rd: %d, rs1: %d, rs2: %d, aux: %d, immf: %d [reference: %s]\n", inst->path, inst->subpath, inst->rd, inst->rs1, inst->rs2, inst->aux, inst->immflag, inst->immref);
}

inst_t decode_inst(int32_t instr)
{
	inst_t in = {0};
	in.path = (instr >> 28) & 0xF;
	in.subpath = (instr >> 21) & 0x7F;
	in.rd = (instr >> 15) & 0x33F;
	in.rs1 = (instr >> 9) & 0x3F;
	in.rs2 = (instr >> 3) & 0x3F;
	in.aux = (instr >> 1) & 0x03;
	in.immflag = instr & 0x1;
	return in;
}

uint32_t encode_inst(inst_t *inst)
{

	return ((inst->path << 28) | (inst->subpath << 21) | (inst->rd << 15) | (inst->rs1 << 9) | (inst->rs2 << 3) | (inst->aux << 1) | inst->immflag);
}

uint64_t encode(uint64_t path, uint64_t subpath, uint64_t rd, uint64_t rs1, uint64_t rs2, uint64_t aux, uint64_t immf)
{
	uint64_t inst = ( (path & 0xF) << 28) | ((subpath &0x7f) << 21) | ((rd &0x1f) << 16) | ((rs1 &0x1f) << 11) | ((rs2 &0x1f) << 6) | ((aux & 0x1f) << 1) | (immf);
	print_bin(inst, 32, 1);
	return inst;
}
void free_inst(inst_t *inst)
{
	free(inst->immref);

}

void invalid_inst(parse_node_t *node, inst_t *inst)
{
	emit_error(TOKEN_ERROR, "not", NULL, NULL);
}

void inst_no_imm(parse_node_t *node, inst_t *inst)
{
	int path = get_path(node->children[0]->tok->lexeme);
	if(path == -1)
	{
		inst->err = not_valid;
		return;
	}

	//printf("path %d\n", path);
	inst->path = path;
	int subpath = get_subpath(path, node->children[1]->tok->lexeme);
	inst->subpath = subpath;

	int rd = get_register(node->children[2]->tok->lexeme);
	int rs1 = get_register(node->children[3]->tok->lexeme);
	int rs2 = get_register(node->children[4]->tok->lexeme);
	inst->rd =rd;
	inst->rs1= rs1;
	inst->rs2= rs2;
	inst->aux = 0;

	inst->err = valid;
	inst->imm = 0;
	inst->immflag = 0;
	inst->immref = NULL;
}

void inst_imm(parse_node_t *node, inst_t *inst)
{
	int path = get_path(node->children[0]->tok->lexeme);
	inst->path = path;
	if(path == -1)
	{
		inst->err = not_valid;
		return;
	}
	//rint_depth(node, 0);

	int subpath = get_subpath(inst->path, node->children[1]->tok->lexeme);
	inst->subpath = subpath;

	int rd = get_register(node->children[2]->tok->lexeme);
	int rs1 = get_register(node->children[3]->tok->lexeme);
	int rs2 = get_register(node->children[4]->tok->lexeme);

	inst->rd =rd;
	inst->rs1= rs1;
	inst->rs2= rs2;
	char *final = node->children[5]->tok->lexeme;

	number_type_t type = get_number_type(final);
	uint64_t imm = 0;
	//printf("final: %s %d\n", final, type);
	if(type == NUM_INT)
	{
		imm = atoi(final);
	}
	if(type == NUM_HEX)
	{
		imm = convert_to_hex(final);
	}
	if(type == NUM_OCT)
	{
		imm = convert_to_oct(final);
	}

	if(type == NUM_NONE)
	{

		if(final[0] == '@')
		{
			if(valid_name(final + 1))
			{
				//faking the valid names
				char *dup = strdup(final + 1);
				inst->immref = dup;
				inst->ref_type = INST_REF_LOCAL;
			}
			else
			{

				inst->err = not_valid;
			}
		}

		else if(valid_name(final))
		{
			char *dup = strdup(final);
			inst->immref = dup;
			inst->ref_type = INST_REF_GLOBAL;
		}
		else
		{
			inst->err = not_valid;
		}
	}

	//printf("%s %d\n", final, imm);
	inst->imm = imm;
	inst->immflag = 1;
	inst->err = valid;
}


data_holder_t decode_string(parse_node_t *head)
{
	size_t buffer_alloc = 100, buffer_size = 0;
	data_holder_t holder = {0};

	uint8_t *buffer = CALLOC(buffer_alloc, uint8_t);

	//the prefill into a long string
	for(int i = 0; i < head->child_count; ++i)
	{
		parse_node_t *child = head->children[i];
		if(child->kind == NODE_LITERAL)
		{
			if(child->tok->type == TOK_STRING)
			{
				int size = strlen(child->tok->lexeme);
				//fill in string
				for(int i = 1; i < size - 1; ++i)
				{

					if(buffer_size + 1 > buffer_alloc)
					{
						buffer = REALLOC(buffer, buffer_alloc *= 2, uint8_t);
					}
					buffer[buffer_size++] = child->tok->lexeme[i];
				}



			}
			else
			{
				printf("parse node is not a string %s\n", child->tok->lexeme);
			}
		}
	}

	if(buffer_alloc <= buffer_size + 1)
	{
		buffer = REALLOC(buffer, buffer_alloc *= 2, uint64_t);

	}
	buffer[buffer_size++] = 0;

	//printf("buffer: %s\n", buffer);
	size_t mod = buffer_size % 8;
	size_t len = buffer_size / 8;
	if(mod > 0)
	{
		len++;
	}

	uint64_t *actual_data = CALLOC(len, uint64_t);

	memcpy(actual_data, buffer, buffer_size);
	free(buffer);
	holder.words = actual_data;
	holder.words_len = len;
	return holder;
}

data_holder_t decode_integer(parse_node_t *head)
{
	data_holder_t holder = {0};
	size_t word_size = head->child_count;
	holder.words = CALLOC(word_size, uint64_t);
	holder.words_len = word_size;

	for(int i = 0; i < word_size; ++i)
	{

		char *tok = head->children[i]->tok->lexeme;
		number_type_t type = get_number_type(tok);
		int64_t imm = 0;
		if(type == NUM_INT)
		{
			imm = atoi(tok);
		}
		if(type == NUM_HEX)
		{
			imm = convert_to_hex(tok);
		}
		if(type == NUM_OCT)
		{
			imm = convert_to_oct(tok);
		}

		holder.words[i] = imm;
	}

	return holder;

}

mop_t create_mop(parse_node_t *node)
{
	mop_t mop;

	char *mop_tok_id = strdup(node->tok->lexeme);

	mop.mop = get_mop_code(mop_tok_id);
	mop.mop_id = mop_tok_id;
	mop.expressions = node->children;
	mop.expressions_len = node->child_count;


	switch(mop.mop)
	{
		case MOP_STRING:
			mop.holder = decode_string(node);
			break;
		case MOP_I16:
			mop.holder = decode_integer(node);
			break;
		case MOP_I32:
			mop.holder = decode_integer(node);

			break;
		case MOP_I8:
			mop.holder = decode_integer(node);

			break;
		case MOP_I64:
			mop.holder = decode_integer(node);
			break;
		default:
			break;
	}

	return mop;
}


inst_t create_instruction(parse_node_t *node)
{

	//printf("line: %s\n", line);


	inst_t inst = {0};
	//clear this
	inst.line = node->tok->locale.row;
	inst.err = not_valid;
	inst.ref_type = INST_REF_NONE;

	if(node->child_count != 6 && node->child_count!= 5)
	{
		//printf("invalid\n");
		invalid_inst(node, &inst);
	}
	else if(node->child_count == 5)
	{
		//printf("no imm");

		inst_no_imm(node, &inst);
	}
	else if(node->child_count == 6)
	{
		//printf("imm\n");

		inst_imm(node, &inst);
	}
	return inst;
}

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
		"x15", "t10"
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
		"x32",
		"x33",
		"x34",
		"x35",
		"x36",
		"x37",
		"x38",
		"x39",
		"x40",
		"x41",
		"x42",
		"x43",
		"x44",
		"x45",
		"x46",
		"x47",
		"x48",
		"x49",
		"x50",
		"x51",
		"x52",
		"x53",
		"x54",
		"x55",
		"x56",
		"x57",
		"x58",
		"x59",
		"x60",
		"x61",
		"x62",
		"x63",
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
		32,
		33,
		34,
		35,
		36,
		37,
		38,
		39,
		40,
		41,
		42,
		43,
		44,
		45,
		46,
		47,
		48,
		49,
		50,
		51,
		52,
		53,
		54,
		55,
		56,
		57,
		58,
		59,
		60,
		61,
		62,
		63
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

mop_id_t get_mop_code(char *keyword)
{
	const char *const mops[] = {
		"I64",
		"I32",
		"I16",
		"I8",
		"FLOAT",
		"DOUBLE",
		"ALIGN",
		"STRING",
		"MEM",
		"PTR",
	};

	mop_id_t ids[] = {
		MOP_I64,
		MOP_I32,
		MOP_I16,
		MOP_I8,
		MOP_FLOAT,
		MOP_DOUBLE,
		MOP_ALIGN,
		MOP_STRING,
		MOP_MEM,
		MOP_PTR,
	};

	int code = determine_code(keyword, mops, ARYSIZE(mops));
	if(code != -1)
	{

		return ids[code];
	}
	else
	{
		printf("not a valid mop code %s\n", keyword);
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
		"ceq"
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
