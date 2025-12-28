#ifndef _ISA_HEADER__
#define _ISA_HEADER__

#include <stdint.h>
#include "isa.h"


typedef struct wcpu_tagged_instruction
{
	//instruction
	inst_t inst;
	int epoch;
	int edge;
	int rs1_epoch;
	int rs2_epoch;
	int rs3_epoch;
	int sidestream_value;
}tagged_inst_t;

#endif