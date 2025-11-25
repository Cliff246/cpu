#include "flags.h"
#include "alu.h"
#include <stdint.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>


part_ptr_t wcpu_alu_generate(void)
{
	alu_t *alu = calloc(1, sizeof(alu_t));

	part_ptr_t pptr;
	pptr.alu = alu;
}

void wcpu_alu_upadate(part_t *alu)
{

}


//just some nice macros
//
/*

static uint64_t arithmetic_shift_right(uint64_t value, uint64_t shift)
{
	uint64_t mask = 0;
	if (value & (1ULL << 63))
	{ // if sign bit set
		mask = (~0ULL) << (64 - shift);
	}
	return (value >> shift) | mask;
}


#define INST(x) static void _alu_ ## x(alu_t *alu)

#define DONE alu->complete = 1; return
#define CYCLE alu->cycle += 1

#define FLAG(x,y) ((alu->flag)? x : y)

#define DEST alu->regdest
#define RS1 alu->reg1
#define RS2 alu->reg2
#define IMM alu->reg3

#define TRI(op) DEST = RS1 op RS2 op IMM; DONE
#define OPT(op) DEST = RS1 op FLAG( IMM , RS2); DONE
#define BIN(op1, op2) DEST = RS1 op1 RS2 op2 IMM; DONE

INST(ADD)
{
	//rd = rs1 + rs2 + imm
	TRI(+);
}

INST(SUB)
{
	//rd = rs1 - rs2 - imm
	TRI(-);
}
INST(SUBU)
{
	DEST = (uint64_t)RS1 - (uint64_t)RS2 - (uint64_t)IMM;
	DONE;
}

INST(AND)
{
	//printf("%lld %lld %lld\n", RS1, IMM, RS1 & IMM);
	OPT(&);
	//printf("%lld\n", DEST);
}
INST(OR)
{
	OPT(|);
}

INST(XOR)
{
	OPT(^);
}

INST(SLL)
{
	OPT(<<);
}
INST(SRL)
{
	OPT(>>);
}
INST(SRA)
{
	DEST = arithmetic_shift_right(RS1, FLAG(RS2, IMM) & 0x3F);
	DONE;

}
INST(DIV)
{
	//printf("divide\n");
	int64_t divident = FLAG(IMM, RS2);
	//printf("divide %d %d %d\n", RS1, divident, RS1 / divident);

	if(divident == 0)
	{
		DEST = 0;
	   	DONE;
	}
	else
	{
		DEST = RS1 / divident;
		DONE;
	}
}
INST(MUL)
{
	BIN(*, +);
}
INST(REM)
{
	OPT(%);
}
INST(MULHI)
{
	uint64_t 	other = FLAG(RS2, IMM);
	uint64_t	a_lo = (uint32_t)RS1;
	uint64_t	a_hi = RS1 >> 32;
	uint64_t	b_lo = (uint32_t)other;
	uint64_t	b_hi = other >> 32;

	uint64_t	a_x_b_hi =  a_hi * b_hi;
	uint64_t	a_x_b_mid = a_hi * b_lo;
	uint64_t	b_x_a_mid = b_hi * a_lo;
	uint64_t	a_x_b_lo =  a_lo * b_lo;
	uint64_t	carry_bit = ((uint64_t)(uint32_t)a_x_b_mid +
						 (uint64_t)(uint32_t)b_x_a_mid +
						 (a_x_b_lo >> 32) ) >> 32;

	uint64_t	multhi = a_x_b_hi +
					 (a_x_b_mid >> 32) + (b_x_a_mid >> 32) +
					 carry_bit;
	DEST = multhi;
	DONE;
}
INST(DIVU)
{

	uint64_t 	other = FLAG(RS2, IMM);
	if(other == 0)
	{
		DEST = 0;
		DONE;
	}
	else
	{
		uint64_t a = RS1 & 0x7FFFFFFFFFFFFFFFULL;
		uint64_t b = other & 0x7FFFFFFFFFFFFFFFULL;
		DEST = a / b;
		DONE;
	}
}

INST(MULU)
{
	uint64_t a = (uint64_t)RS1;
	uint64_t b = (uint64_t)RS2;
	uint64_t c = (uint64_t)IMM;
	DEST = a * b + c;
	DONE;
}
INST(MULUS)
{
	uint64_t a = (uint64_t)RS1;
	int64_t b = (int64_t)RS2;
	int64_t c = (int64_t)IMM;
	DEST = a * b + c;
	DONE;

}

INST(NOT)
{
	DEST = ~(RS1);
	DONE;

}

INST(CLT)
{
	OPT(<);
}
INST(CLE)
{
	OPT(<=);
}
INST(CLTU)
{
	DEST = (uint64_t)RS1 < (uint64_t)FLAG(RS1, RS2);
	DONE;
}
INST(CEQ)
{
	OPT(==);
}
INST(CNE)
{
	OPT(!=);
}

INST(NONE) {DONE;}

void (*alu_fn[255])(alu_t *alu) = {_alu_NONE};

#define SET(x) alu_fn[ALU_ ## x] = _alu_ ## x

void set_alu_instructions(void)
{
	SET(ADD);
	SET(SUB);
	SET(SUBU);
	SET(AND);
	SET(OR);
	SET(XOR);
	SET(SLL);
	SET(SRL);
	SET(SRA);
	SET(DIV);
	SET(MUL);
	SET(REM);
	SET(MULHI);
	SET(MULU);
	SET(MULUS);
	SET(NOT);
	SET(CLT);
	SET(CLE);
	SET(CLTU);
	SET(CEQ);
	SET(CNE);
}

#undef SET
#undef INST
//

#undef DONE
#undef CYCLE

#undef FLAG

#undef DEST
#undef RS1
#undef RS2
#undef IMM
#undef TRI
#undef OPT
#undef BIN

alu_t *create_alu(void)
{
	static bool alu_set = false;
	if(alu_set == false)
	{
		set_alu_instructions();
		alu_set = true;
	}
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
	alu->reg3 = imm;
	alu->regdest = 0;
	alu->subpath = subpath;
	alu->flag = immflag;
	alu->cycle = 0;
	alu->complete = 0;
}

//step through the alu
void alu_step(alu_t *alu)
{
	//printf("%lld %lld %lld\n", alu->reg1, alu->reg2, alu->reg3);

	alu_fn[alu->subpath](alu);
	//printf("dest: %d\n", alu->regdest);

}

//poll the alu
int alu_poll(alu_t *alu)
{
	return alu->complete;
}

*/