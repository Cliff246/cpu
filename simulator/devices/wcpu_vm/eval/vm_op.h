#ifndef __VM_OP_HEADER__
#define __VM_OP_HEADER__

#include "isa.h"

#include <stdint.h>
#include <stdbool.h>





typedef struct 
{
	//instruction grabed;
	uint64_t pc;
	uint64_t ipc;	
	uint64_t cd;
	int32_t inst;
	int64_t imm;

	inst_t op;
}vm_op_t;

#endif
