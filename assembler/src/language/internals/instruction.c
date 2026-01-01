#include "instruction.h"
#include "fileio.h"
#include "commons.h"
#include "lexer.h"
#include "eerror.h"
#include "decoder.h"
#include "parser.h"
#include "strtools.h"
#include <stdio.h>
#include <stdint.h>


void print_inst(inst_t *inst)
{


	if(inst->imm_type == INSTIMM_LITERAL)
	{
		switch(inst->imm.ilit.lit_type)
		{

			default:
				printf("path:%d subpath %d: rs1: %d, rs2: %d, rs3: %d, accflag %d, selflag: %d, realocflag %d, immf: %d [immediate: %lld]\n", inst->path, inst->subpath, inst->rs1, inst->rs2, inst->rs3, inst->accflag, inst->selflag, inst->realocflag, inst->immflag, inst->imm.ilit.lit);

				break;
		}

	}
	else if(inst->imm_type == INSTIMM_REFERENCE)
	{

		if(inst->imm.iref.ref_type == INST_REF_GLOBAL)
		{
			printf("path:%d subpath %d: rs1: %d, rs2: %d, rs3: %d, accflag %d, selflag: %d, realocflag %d, immf: %d [reference: %s]\n", inst->path, inst->subpath, inst->rs1, inst->rs2, inst->rs3, inst->accflag, inst->selflag, inst->realocflag, inst->immflag, inst->imm.iref.ref);

		}
		else
		{
			printf("path:%d subpath %d: rs1: %d, rs2: %d, rs3: %d, accflag %d, selflag: %d, realocflag %d, immf: %d [reference: @%s]\n", inst->path, inst->subpath, inst->rs1, inst->rs2, inst->rs3, inst->accflag, inst->selflag, inst->realocflag, inst->immflag, inst->imm.iref.ref);
		}

	}

	else if(inst->imm_type == INSTIMM_NONE)
	{
		printf("path:%d subpath %d: rs1: %d, rs2: %d, rs3: %d, accflag %d, selflag: %d, realocflag %d, immf: %d\n", inst->path, inst->subpath, inst->rs1, inst->rs2, inst->rs3, inst->accflag, inst->selflag, inst->realocflag, inst->immflag);
	}
}

inst_t decode_inst(uint32_t instr)
{
	inst_t in = {0};
	in.path = (instr >> 29) & 0x7;
	in.subpath = (instr >> 22) & 0x7F;
	in.rs1 = (instr >> 16) & 0x3F;
	in.rs1 = (instr >> 10) & 0x3F;
	in.rs2 = (instr >> 4) & 0x3F;
	in.accflag = (instr >> 3) & 0x01;
	in.selflag = (instr >> 2) & 0x01;
	in.realocflag = (instr >> 1) & 0x01;
	in.immflag = instr & 0x01;
	return in;
}

uint32_t encode_inst(inst_t *inst)
{

	return ((inst->path << 29) | (inst->subpath << 22) | (inst->rs1 << 16) | (inst->rs2 << 10) | (inst->rs3 << 4) | (inst->accflag << 3) | ((inst->selflag << 2)) | ((inst->realocflag << 1)) | inst->immflag);
}


void set_instruction_keyword_error(tok_t *tok, char *keyword)
{

	errelm_line_t line = {.column = get_token_col(tok), .line = get_token_row(tok)};
	errelm_file_t file = {.name = get_filename_from_id(get_token_file(tok))};
	char buffer[1025] = {0};
	sprintf(buffer, "keyword %s is not in not a valid instruction keyword", keyword);
	errelm_t elmline = errelm_create_line_element(line);
	errelm_t elmfile =  errelm_create_file_element(file);
	//printf("emit stage\n");
	emit_error(TOKEN_ERROR, buffer, 2, elmfile, elmline);
	//printf("emit stage2\n");
}

void set_instruction_reference_error(tok_t *tok, char *reference)
{
	errelm_line_t line = {.column = get_token_col(tok), .line = get_token_row(tok)};
	errelm_file_t file = {.name = get_filename_from_id(get_token_file(tok))};
	char buffer[1025] = {0};
	sprintf(buffer, "reference %s is not in not a valid assembler keyword", reference);
	errelm_t elmline = errelm_create_line_element(line);
	errelm_t elmfile =  errelm_create_file_element(file);
	//printf("emit stage\n");
	emit_error(TOKEN_ERROR, buffer, 2, elmfile, elmline);
}


void free_inst(inst_t *inst)
{
	if(inst->imm_type == INSTIMM_REFERENCE)
	{
		free(inst->imm.iref.ref);
	}
}


void invalid_inst(parse_node_t *node, inst_t *inst)
{
	emit_error(TOKEN_ERROR, "not", 0, NULL);
}



int fill_instruction_start(parse_node_t *node, inst_t *inst)
{
	char *path_str = node->children[0]->tok->lexeme;
	int path = get_path(path_str);
	inst->path = path;
	if(path == -1)
	{



		set_instruction_keyword_error(node->children[0]->tok, path_str);
		inst->imm_type == INSTIMM_ERROR;
		return 0;
	}
	//rint_depth(node, 0);

	int subpath = get_subpath(inst->path, node->children[1]->tok->lexeme);
	if(subpath == -1)
	{
		set_instruction_keyword_error(node->children[1]->tok, node->children[1]->tok->lexeme);
		inst->imm_type == INSTIMM_ERROR;
		return 1;

	}
	inst->subpath = subpath;
	int offset = 2;

	if(node->children[offset]->kind == NODE_EXCLAIM)
	{
		inst->accflag = 1;
		offset++;
	}


	//printf("<%s>\n",node->children[offset]->tok->lexeme);

	int rd = get_register(node->children[offset]->tok->lexeme);
	if(rd == -1)
	{
		set_instruction_keyword_error(node->children[offset]->tok, node->children[offset]->tok->lexeme);
		inst->imm_type == INSTIMM_ERROR;
		return offset;

	}
	offset++;

	//printf("<%s>\n",node->children[offset]->tok->lexeme);
	int rs1 = get_register(node->children[offset]->tok->lexeme);
	if(rs1 == -1)
	{
		set_instruction_keyword_error(node->children[offset]->tok,  node->children[offset]->tok->lexeme);
		inst->imm_type == INSTIMM_ERROR;
		return offset;

	}
	int rs2 = 0;


	offset++;
	//printf("<%s>\n",node->children[offset]->tok->lexeme);


	char *rs2_str = node->children[offset]->tok->lexeme;


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
	if(rs2 == -1)
	{
		set_instruction_keyword_error(node->children[offset]->tok, node->children[offset]->tok->lexeme);
		inst->imm_type == INSTIMM_ERROR;
		return offset;

	}
	offset++;
	inst->rs1 = rd;
	inst->rs2 = rs1;
	inst->rs3 = rs2;
	return offset;
}

void inst_no_imm(parse_node_t *node, inst_t *inst, int offset)
{

	if(inst->imm_type != INSTIMM_ERROR)
	{

		inst->imm_type = INSTIMM_NONE;
		inst->immflag = 0;
	}


}

void inst_imm(parse_node_t *node, inst_t *inst, int offset)
{

	if(inst->imm_type == INSTIMM_ERROR)
	{
		return;
	}

	char *final = node->children[offset]->tok->lexeme;

	number_type_t type = get_number_type(final);
	uint64_t imm = 0;
	//printf("final: %s %d\n", final, type);
	inst->realocflag = 0;
	inst->immflag = 1;
	if(type != NUM_NONE)
	{
		if(type == NUM_INT)
		{
			imm = atoi(final);
		}
		else if(type == NUM_HEX)
		{
			imm = convert_to_hex(final);
		}
		else if(type == NUM_OCT)
		{
			imm = convert_to_oct(final);
		}
		inst->imm_type = INSTIMM_LITERAL;
		inst->imm.ilit.lit = imm;
		inst->imm.ilit.lit_type = INST_LIT_UNSET;
	}
	else if(!strcmp(final, "acc"))
	{
		inst->imm_type = INSTIMM_NONE,
		inst->immflag = 0;
		inst->realocflag = 1;
	}
	else
	{

		if(final[0] == '@')
		{
			if(valid_name(final + 1))
			{
				//faking the valid names
				char *dup = strdup(final + 1);
				inst->imm.iref.ref = dup;
				inst->imm.iref.ref_type = INST_REF_LOCAL;

			}
			else
			{

			}
		}
		else if(valid_name(final))
		{
			char *dup = strdup(final);
			//printf("%s\n", dup);
			inst->imm.iref.ref = dup;
			inst->imm.iref.ref_type = INST_REF_GLOBAL;
		}
		else
		{

		}
		inst->imm_type = INSTIMM_REFERENCE;

	}

	//printf("%s %d\n", final, imm);

}


inst_t create_instruction(parse_node_t *node)
{

	//printf("line: %s\n", line);


	inst_t inst = {0};
	//clear this

	int offset = fill_instruction_start(node, &inst);
	if(offset < node->child_count)
	{
		//printf("instruction imm %d child %d\n", offset, node->child_count);

		inst_imm(node, &inst, offset);

	}
	else
	{
		//printf("instruction no imm %d child %d\n", offset, node->child_count);

		inst_no_imm(node, &inst, offset);

	}

	return inst;
}