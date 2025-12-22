#include "isa.h"
#include <stdio.h>


inst_t decode_inst(int32_t instr)
{
	inst_t in = {0};
	in.path = (instr >> 29) & 0x3f;
	in.subpath = (instr >> 22) & 0x7F;
	in.rs1 = (instr >> 16) & 0x3F;
	in.rs2 = (instr >> 10) & 0x3F;

	in.rs3 = (instr >> 4) & 0x3F;
	in.accflag = (instr >> 3) & 0x01;
	in.selflag = (instr >> 2) & 0x01;
	in.reallocflag = (instr >> 1) & 0x01;
	in.immflag = instr & 0x1;
	return in;
}

int32_t encode_inst(inst_t *inst)
{

	return ((inst->path << 29) | (inst->subpath << 22) | (inst->rs1 << 16) | (inst->rs1 << 10) | (inst->rs2 << 4) | (inst->accflag << 3) | (inst->selflag << 2) | (inst->reallocflag << 1) | inst->immflag);
}


void print_inst(inst_t *inst)
{
	inst_t op = *inst;
	printf("p: %d: sp: %d, rd: %d, rs1: %d, rs2: %d,accflag %d, selflag: %d, reallocflag: %d, f: %d\n", op.path, op.subpath, op.rs1, op.rs2, op.rs3,op.accflag, op.selflag, op.reallocflag, op.immflag);
}
