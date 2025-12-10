#ifndef __WCPU_ISA_HEADER__
#define __WCPU_ISA_HEADER__

#include <stdint.h>




typedef struct instruction
{
	char path;
	char subpath;
	char rs1 ;
	char rs2 ;
	char rs3 ;
	char accflag;
	char selflag ;
	char reallocflag ;
	char immflag;
}inst_t;

typedef struct operation
{
	inst_t ins;
	int64_t imm;
}operation_t;

typedef struct wcpu_code_description
{
	uint64_t ct_base;
	uint64_t ct_len;
	uint64_t ins_base;
	uint64_t ins_len;
	uint64_t imm_base;
	uint64_t imm_len;
}wcpu_isa_code_description_t;


inst_t decode_inst(int32_t code);
int32_t encode_inst(inst_t *inst);
void print_inst(inst_t *inst);


#endif