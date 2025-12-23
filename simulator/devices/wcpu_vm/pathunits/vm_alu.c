

#include "vm.h"
#include "vm_alu.h"
#include "flags.h"
#include <stdint.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

//just some nice macros
//

static uint64_t arithmetic_shift_right(uint64_t value, uint64_t shift)
{
	uint64_t mask = 0;
	if (value & (1ULL << 63))
	{ // if sign bit set
		mask = (~0ULL) << (64 - shift);
	}
	return (value >> shift) | mask;
}


#define INST(x) static int64_t _vm_alu_ ## x(vima_t *vm, int64_t lhs, int64_t rhs)


#define LANE1 vm->cpu.pre.lane1
#define LANE2 vm->cpu.pre.lane2
#define LANE3 vm->cpu.pre.lane3




#define ALU_OP(OP) return ((lhs OP rhs))
#define ALU_OP_TYPED(OP,T1, T2) return ((((T1)lhs) OP ((T2)rhs)))


INST(ADD)
{
	//rd = rs1 + rs2 + imm
	ALU_OP(+);
}

INST(SUB)
{
	//rd = rs1 - rs2 - imm
	ALU_OP(-);
}
INST(SUBU)
{
	ALU_OP_TYPED(-, uint64_t, uint64_t);
}

INST(AND)
{
	//printf("and %lld %lld %lld %lld\n", lhs, rhs, lhs & rhs, opp);
	ALU_OP(&);
	//printf("%lld\n", DEST);
}
INST(OR)
{
	ALU_OP(|);
}

INST(XOR)
{
	ALU_OP(^);
}

INST(SLL)
{
	printf("shift left logical:\n");

	ALU_OP(<<);
}
INST(SRL)
{
	printf("shift right logical\n");

	ALU_OP(>>);
}
INST(SRA)
{
	int64_t shift =arithmetic_shift_right(lhs, rhs & 0x3F);
	//printf("shifted: %d\n", shift);
	return shift;
}
INST(DIV)
{
	//printf("divide\n");
	int64_t divident = rhs;
	//printf("divide %d %d %d\n", RS1, divident, RS1 / divident);

	if(divident == 0)
	{
		return 0;
	}
	else
	{
		return (lhs / divident);
	}
}

INST(MUL)
{
	ALU_OP(*);
}
INST(REM)
{
	ALU_OP(%);
}
INST(MULHI)
{
	uint64_t	a_lo = (uint32_t)lhs;
	uint64_t	a_hi = lhs >> 32;
	uint64_t	b_lo = (uint32_t)rhs;
	uint64_t	b_hi = rhs >> 32;

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
	return multhi;
}

INST(DIVU)
{

	if(rhs == 0)
	{
		return 0;
	}
	else
	{
		uint64_t a = lhs & 0x7FFFFFFFFFFFFFFFULL;
		uint64_t b = rhs & 0x7FFFFFFFFFFFFFFFULL;
		return (a / b);
	}
}

INST(MULU)
{
	ALU_OP_TYPED(*, uint64_t, uint64_t);
}
INST(MULUS)
{

	ALU_OP_TYPED(*,int64_t, uint64_t);

}

INST(NOT)
{
	return ~(lhs);
}

INST(CLT)
{
	ALU_OP(<);
}
INST(CLE)
{
	ALU_OP(<=);
}
INST(CLTU)
{
	ALU_OP_TYPED(<, uint64_t, uint64_t);
}
INST(CEQ)
{
	ALU_OP(==);
}
INST(CNE)
{
	ALU_OP(!=);
}

INST(NONE) {}

int64_t (*alu_fn[255])(vima_t *vm, int64_t lhs, int64_t rhs) = {_vm_alu_NONE};

#define SET(x) alu_fn[ALU_ ## x] = _vm_alu_ ## x

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

void vm_cpu_path_alu_init(vima_t *vm)
{
	set_alu_instructions();
}

int64_t vm_cpu_path_alu_exec(vima_t *vm, char subflag, int64_t lane1, int64_t lane2, int64_t lane3, bool swap)
{

	int64_t lhs = lane1;
	int64_t rhs, opp;
	if(swap)
	{
		printf("swaped\n");

		rhs = lane3;
		opp = lane2;
	}
	else
	{
		rhs = lane2;
		opp = lane3;
	}
	//before
	int64_t result = alu_fn[subflag](vm, lhs, rhs);
	//after
	result = result + opp;



	printf("alu:%d (%lld %lld %lld) = %lld\n", subflag, lane1, lane2, lane3, result);
	return result;
}


vm_op_status_t vm_ALU_ADD_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{

}

vm_op_status_t vm_ALU_SUB_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{

}

vm_op_status_t vm_ALU_SUBU_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{

}

vm_op_status_t vm_ALU_AND_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{

}

vm_op_status_t vm_ALU_OR_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{

}



vm_op_status_t vm_ALU_XOR_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{

}

vm_op_status_t vm_ALU_SLL_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{

}

vm_op_status_t vm_ALU_SRL_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{

}

vm_op_status_t vm_ALU_SRA_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{

}

vm_op_status_t vm_ALU_DIV_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{

}

vm_op_status_t vm_ALU_REM_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{

}

vm_op_status_t vm_ALU_MUL_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{

}

vm_op_status_t vm_ALU_DIVU_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{

}

vm_op_status_t vm_ALU_MULHI_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{

}

vm_op_status_t vm_ALU_MULU_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{

}

vm_op_status_t vm_ALU_MULUS_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{

}

vm_op_status_t vm_ALU_NOT_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{

}

vm_op_status_t vm_ALU_CLT_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{

}

vm_op_status_t vm_ALU_CLE_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{

}

vm_op_status_t vm_ALU_CLTU_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{

}

vm_op_status_t vm_ALU_CEQ_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{

}

vm_op_status_t vm_ALU_CNE_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{

}