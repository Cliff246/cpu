#include "alu.h"
#include "flags.h"
#include <stdint.h>
#include <errno.h>
#include <stdlib.h>


void _alu_add(alu_t *alu)
{
	alu->regdest = alu->reg1 + alu->reg2 + alu->reg3;
	alu->complete = 1;
}
void _alu_sub(alu_t *alu)
{
	
	alu->regdest = alu->reg1 - alu->reg2 - alu->reg3;
	alu->complete = 1;
}

void (*alu_fn[])(alu_t *alu) = {
	_alu_add,
	_alu_sub
};

alu_t *create_alu(void)
{
	alu_t *alu = (alu_t *)malloc(sizeof(alu_t));	
	if(!alu)
	{
		errno = ENOMEM;
		return NULL;
	}
	return alu;
}


//sumbit work
void alu_submit(alu_t *alu, char subpath, int64_t rs1, int64_t rs2, int64_t imm, char immflag)
{
	alu->reg1 = rs1;
	alu->reg2 = rs2;
	alu->reg3 = (immflag)? imm : 0;
	alu->regdest = 0;
	alu->subpath = subpath;	
	alu->flag = immflag;
	alu->cycle = 0;
	alu->complete = 0;
}

//step through the alu
void alu_step(alu_t *alu)
{
	alu_fn[alu->subpath](alu);	
}

//poll the alu
int alu_poll(alu_t *alu)
{
	return alu->complete;
}

