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

typedef enum inst_lit_type
{
	INST_LIT_NONE,
	INST_LIT_UNSET,
	INST_LIT_SIGNED_INT,
	INST_LIT_UNSIGNED_INT,
	INST_LIT_FLOAT,

}inst_lit_type_t;

typedef enum inst_imm_type
{
	INSTIMM_NONE,
	INSTIMM_ERROR,
	INSTIMM_LITERAL,
	INSTIMM_REFERENCE,
}inst_imm_type_t;



typedef struct imm_reference
{
	char *ref;
	inst_ref_type_t ref_type;

}imm_ref_t;

typedef struct imm_literal
{
	int64_t lit;
	inst_lit_type_t lit_type;
}imm_lit_t;





typedef struct instruction
{
	char path;
	char subpath;
	char rs1;
	char rs2;
	char rs3;
	char accflag;
	char realocflag;
	char selflag;
	char immflag;
	union
	{
		imm_ref_t iref;
		imm_lit_t ilit;
	}imm;
	inst_imm_type_t imm_type;
}inst_t;


inst_t decode_inst(uint32_t code);
uint32_t encode_inst(inst_t *inst);
uint64_t encode(uint64_t path, uint64_t subpath, uint64_t rd, uint64_t rs1, uint64_t rs2, uint64_t aux, uint64_t immf);
void print_inst(inst_t *inst);
void free_inst(inst_t *inst);
inst_t create_instruction(parse_node_t *node);


void invalid_inst(parse_node_t *node, inst_t *inst);


void inst_no_imm(parse_node_t *node, inst_t *inst, int offset);


void inst_imm(parse_node_t *node, inst_t *inst, int offset);







#endif