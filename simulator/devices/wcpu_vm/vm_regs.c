

#include "vm.h"
#include "vm_regs.h"
#include "vm_utils.h"
#include "isa.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>


char *reg_codes[WCPU_REGISTER_COUNT] =
{
	"zero",	"a0",	"a1",	"a2",	"a3",	"t0",	"t1",	"t2",	"t3",	"t4",	"t5",	"t6",	"t7",	"t8",	"t9",	"t10",	"g0",	"g1",	"g2",	"g3",	"g4",	"g5",	"g6",	"g7",	"s0",	"s1",	"s2",	"s3",	"s4",	"sys",	"aux0",	"aux1",	"a4",	"a5",	"a6",	"a7",	"a8",	"a9",	"s5",	"s6",	"s7",	"s8",	"s9",	"s10",	"s11",	"s12",	"s13",	"s14",	"t11",	"t12",	"t13",	"t14",	"six",	"siy",	"siz",	"tix",	"tiy",	"tiz",	"t18",	"t19",	"t20",	"t21",	"t22",	"acc",
};

void vm_cpu_print_regs(vima_t *vm)
{
	for (int i = 0; i < 8; ++i)
	{
		for(int s = 0; s < 8; ++s)
		{
			int reg_number =(i * 8) + s;
			printf("[%s]=%lld ", reg_codes[reg_number], vm->regs.regs[reg_number]);
		}
		printf("\n");
	}
}

