#include "instruction.h"

#include "commons.h"
#include "lexer.h"
#include "eerror.h"
#include "decoder.h"
#include <stdio.h>
#include <stdint.h>
#include "parser.h"
#include "strtools.h"

void print_inst(inst_t *inst)
{
	printf("path:%d subpath %d: rd: %d, rs1: %d, rs2: %d, selflag: %d, realocflag %d, immf: %d [reference: %s]\n", inst->path, inst->subpath, inst->rd, inst->rs1, inst->rs2, inst->selflag, inst->realocflag, inst->immflag, inst->immref);
}

inst_t decode_inst(uint32_t instr)
{
	inst_t in = {0};
	in.path = (instr >> 28) & 0xF;
	in.subpath = (instr >> 21) & 0x7F;
	in.rd = (instr >> 15) & 0x3F;
	in.rs1 = (instr >> 9) & 0x3F;
	in.rs2 = (instr >> 3) & 0x3F;
	in.selflag = (instr >> 2) & 0x01;
	in.realocflag = (instr >> 1) & 0x01;
	in.immflag = instr & 0x01;
	return in;
}

uint32_t encode_inst(inst_t *inst)
{

	return ((inst->path << 28) | (inst->subpath << 21) | (inst->rd << 15) | (inst->rs1 << 9) | (inst->rs2 << 3) | ((inst->selflag << 2)) | ((inst->realocflag << 1) & 0x1) | inst->immflag);
}


void free_inst(inst_t *inst)
{
	if(inst->immref)
	{
		free(inst->immref);
	}
}

void invalid_inst(parse_node_t *node, inst_t *inst)
{
	emit_error(TOKEN_ERROR, "not", 0, NULL);
}

void inst_no_imm(parse_node_t *node, inst_t *inst)
{
	int path = get_path(node->children[0]->tok->lexeme);
	if(path == -1)
	{
		return;
	}

	//printf("path %d\n", path);
	inst->path = path;
	int subpath = get_subpath(path, node->children[1]->tok->lexeme);
	inst->subpath = subpath;

	int rd = get_register(node->children[2]->tok->lexeme);
	int rs1 = get_register(node->children[3]->tok->lexeme);
	int rs2 = 0;


	char *rs2_str = node->children[4]->tok->lexeme;

	if(rs2_str[0] == '#')
	{
		int value = atoi(rs2_str + 1);
		rs2 = value;
		inst->selflag = 1;
	}
	else
	{
		rs2 = get_register(rs2_str);

	}
	inst->rd =rd;
	inst->rs1= rs1;
	inst->rs2= rs2;
	inst->realocflag = 0;


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
		return;
	}
	//rint_depth(node, 0);

	int subpath = get_subpath(inst->path, node->children[1]->tok->lexeme);
	inst->subpath = subpath;

	int rd = get_register(node->children[2]->tok->lexeme);


	int rs1 = get_register(node->children[3]->tok->lexeme);
	int rs2 = 0;


	char *rs2_str = node->children[4]->tok->lexeme;

	if(rs2_str[0] == '#')
	{
		int value = atoi(rs2_str + 1);
		rs2 = value;

		inst->selflag = 1;
	}
	else
	{
		rs2 = get_register(rs2_str);

	}

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
		//printf("%s\n", final);

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

			}
		}

		else if(valid_name(final))
		{
			char *dup = strdup(final);
			//printf("%s\n", dup);
			inst->immref = dup;

		}
		else
		{
		}
	}

	//printf("%s %d\n", final, imm);
	inst->realocflag = 0;
	inst->imm = imm;
	inst->immflag = 1;
}


inst_t create_instruction(parse_node_t *node)
{

	//printf("line: %s\n", line);


	inst_t inst = {0};
	//clear this
	inst.line = node->tok->locale.row;
	inst.ref_type = INST_REF_GLOBAL;

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