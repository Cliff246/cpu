#ifndef __WCPU_ISA_HEADER__
#define __WCPU_ISA_HEADER__

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define WCPU_REGISTER_COUNT 64

#define CODE_DESCRIPTION_SIZE 6

typedef struct instruction
{
	char path;
	char subpath;
	char rs1 ;
	char rs2 ;
	char rs3 ;
	char modeflag;
	char selflag ;
	char immflag;
}inst_t;



inst_t decode_inst(int32_t code);
int32_t encode_inst(inst_t *inst);
void print_inst(inst_t *inst);

#endif
