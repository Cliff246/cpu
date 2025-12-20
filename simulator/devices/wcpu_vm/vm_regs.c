
/*
#include "vm.h"
#include "vm_wcpu.h"
#include "cpu_utils.h"
#include "isa.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>





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

void vm_cpu_print_regs(vima_t *vm)
{
	for (int i = 0; i < 64; ++i)
	{
		printf("reg:%d = %lld\n", i, vm->cpu.user.regs[i]);
	}
}
*/
