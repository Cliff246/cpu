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

inst_t decode_inst(int32_t code);
int32_t encode_inst(inst_t *inst);
void print_inst(inst_t *inst);


#endif