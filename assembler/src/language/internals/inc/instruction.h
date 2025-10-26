#ifndef __INSTRUCTION_HEADER__
#define __INSTRUCTION_HEADER__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

typedef enum inst_ref_type
{
	INST_REF_NONE,
	INST_REF_LOCAL,
	INST_REF_GLOBAL
}inst_ref_type_t;

typedef struct instruction
{
	uint32_t path;
	uint32_t subpath;
	uint32_t rd;
	uint32_t rs1;
	uint32_t rs2;
	char realocflag;
	char selflag;
	char immflag;

	uint64_t line;

	int64_t imm;
	char *linestr;

	char *immref;
	inst_ref_type_t ref_type;
}inst_t;


inst_t decode_inst(uint32_t code);
uint32_t encode_inst(inst_t *inst);
uint64_t encode(uint64_t path, uint64_t subpath, uint64_t rd, uint64_t rs1, uint64_t rs2, uint64_t aux, uint64_t immf);
void print_inst(inst_t *inst);
void free_inst(inst_t *inst);
inst_t create_instruction(parse_node_t *node);


void invalid_inst(parse_node_t *node, inst_t *inst);


void inst_no_imm(parse_node_t *node, inst_t *inst);


void inst_imm(parse_node_t *node, inst_t *inst);







#endif