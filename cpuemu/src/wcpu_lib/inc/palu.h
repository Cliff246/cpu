#ifndef __ALU__
#define __ALU__

#include <stdint.h>

typedef struct
{
	int64_t reg1, reg2, reg3;
	int64_t regdest;
	char flag;
	char subpath;
	int cycle;
	int complete;
}alu_t;

void set_alu_instructions(void);

alu_t *create_alu(void);

void alu_submit(alu_t *alu, char subpath, int64_t rs1, int64_t rs2, int64_t imm, char immflag);

void alu_step(alu_t *alu);

int alu_poll(alu_t *alu);

#endif
