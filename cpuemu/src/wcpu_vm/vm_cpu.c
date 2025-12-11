
#include "vm.h"
#include "vm_cpu.h"
#include "vm_cpu_utils.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>


#include "vm_cpu_utils.h"




void vm_cpu_step(vima_t *vm)
{
//fetch
	int64_t imm = vm_load(vm, vm->cpu.user.code_desc.ipc + vm->cpu.user.code_desc.ipc_base);
	uint32_t instbin = vm_get_inst_at_pc_address(vm, vm->cpu.user.code_desc.pc);

	printf("pc:%d ipc: %d\n",vm->cpu.user.code_desc.pc, vm->cpu.user.code_desc.ipc);

//
	vm_inst_t inst = decode_inst(instbin);

	int64_t dest = 0;
	int64_t l1, l2, l3;
	int64_t sidestream = 0;
	bool swaps = true;
	if(inst.immflag && inst.reallocflag)
	{
		printf("both high\n");
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
		swaps = false;
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


	printf("%d\n", swaps);
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



vm_inst_t decode_inst(int32_t instr)
{
	vm_inst_t in = {0};
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

int32_t encode_inst(vm_inst_t *inst)
{

	return ((inst->path << 29) | (inst->subpath << 22) | (inst->rs1 << 16) | (inst->rs1 << 10) | (inst->rs2 << 4) | (inst->accflag << 3) | (inst->selflag << 2) | (inst->reallocflag << 1) | inst->immflag);
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