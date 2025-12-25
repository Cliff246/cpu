

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

/*



void vm_cpu_step(vima_t *vm)
{
//fetch
	int64_t imm = vm_load(vm, vm->cpu.user.code_desc.ipc + vm->cpu.user.code_desc.ipc_base);
	uint32_t instbin = vm_get_inst_at_pc_address(vm, vm->cpu.user.code_desc.pc);

	printf("pc:%d ipc: %d\n",vm->cpu.user.code_desc.pc, vm->cpu.user.code_desc.ipc);

//
	inst_t inst = decode_inst(instbin);

	int64_t dest = 0;
	int64_t l1, l2, l3;
	int64_t sidestream = 0;
	bool swaps = inst.immflag;
	if(inst.immflag && inst.reallocflag)
	{
		sidestream = vm_load(vm, imm);
	}
	else if(inst.immflag)
	{
		sidestream = imm;
	}
	else if(inst.reallocflag)
	{
		sidestream = vm_cpu_get_reg(vm, 63);
	}
	else
	{
		sidestream = 0;
	}


	int64_t rs3_val = (inst.selflag)? inst.rs3 : vm_cpu_get_reg(vm, inst.rs3);

	if(inst.accflag)
	{
		dest = 63;
		l1 = vm_cpu_get_reg(vm, inst.rs1);
		l2 = vm_cpu_get_reg(vm, inst.rs2);
		l3 = rs3_val + sidestream;

	}
	else
	{
		dest = inst.rs1;
		l1 = vm_cpu_get_reg(vm, inst.rs2);
		l2 = rs3_val;
		l3 = sidestream;
	}


	//printf("inst: %d %d %d %d %d\n", inst.path, inst.subpath, l1, l2,l3);
  	int64_t sets = vm_cpu_path_fn[inst.path].exec(vm, inst.subpath, l1, l2, l3, swaps);
	//printf("value:%d\n", sets);
	vm_cpu_set_reg(vm,dest,sets);

	if(!vm->cpu.has_jumped)
	{
		vm_inc_pc(vm, 1);
		vm_inc_ipc(vm, (inst.immflag) ? 1 : 0);
	}
	vm->cpu.has_jumped = false;



}



void vm_cpu_set_inital_cd(vima_t *vm)
{
	vm->cpu.user.code_desc.ct_base= vm->mem.mem[0];
	vm->cpu.user.code_desc.ct_len = vm->mem.mem[1];
	vm->cpu.user.code_desc.pc_base = vm->mem.mem[2];
	vm->cpu.user.code_desc.pc_len = vm->mem.mem[3];
	vm->cpu.user.code_desc.ipc_base = vm->mem.mem[4];
	vm->cpu.user.code_desc.ipc_len = vm->mem.mem[5];

}
*/

char *reg_codes[64] =
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

