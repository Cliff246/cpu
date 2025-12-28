


#include "vm.h"
#include "vm.h"
#include "vm_utils.h"
#include "flags.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>








/*

void vm_push_scd(vima_t *vm, vm_reg_cd_t desc)
{
	uint64_t stack_cd =  vm->regs.stack.scd;


	vm_store(vm, stack_cd, desc.cd_ptr);
	vm_store(vm, stack_cd + 1, desc.ct_base);
	vm_store(vm, stack_cd + 2, desc.ct_len);
	vm_store(vm, stack_cd + 3, desc.pc_base);
	vm_store(vm, stack_cd + 4, desc.pc_len);
	vm_store(vm, stack_cd + 5, desc.ipc_base);
	vm_store(vm, stack_cd + 6, desc.ipc_len);
	vm_store(vm, stack_cd + 7, desc.pc);
	vm_store(vm, stack_cd + 8, desc.ipc);


	vm->regs.stack.scd += 9;
}

vm_reg_cd_t vm_pop_scd(vima_t *vm)
{
	vm->regs.stack.scd -= 9;
	uint64_t stack_cd =  vm->regs.stack.scd ;


	vm_reg_cd_t desc = {
		.cd_ptr = vm_load(vm, stack_cd),
		.ct_base = vm_load(vm, stack_cd + 1),
		.ct_len = vm_load(vm, stack_cd + 2),
		.pc_base = vm_load(vm, stack_cd + 3),
		.pc_len = vm_load(vm, stack_cd + 4),
		.ipc_base = vm_load(vm, stack_cd + 5),
		.ipc_len = vm_load(vm, stack_cd +6),
		.pc = vm_load(vm, stack_cd + 7),
		.ipc = vm_load(vm, stack_cd + 8)

	};

	return desc;
}

vm_reg_cd_t vm_get_cd_from_address(vima_t *vm, uint64_t address)
{
	int64_t cdtb_ptr = vm_load(vm, address);
	int64_t cdtb_len = vm_load(vm, address + 1);
	int64_t inst_ptr = vm_load(vm, address + 2);
	int64_t inst_len = vm_load(vm, address + 3);
	int64_t imm_ptr = vm_load(vm, address + 4);
	int64_t imm_len = vm_load(vm, address + 5);
	//set_pc(inst_ptr);
	//set_ipc(imm_ptr);
	vm_reg_cd_t frame = {
		.cd_ptr = address,
		.ct_base = cdtb_ptr,
		.ct_len = cdtb_len,
		.ipc_base = imm_ptr,
		.ipc_len = imm_len,
		.pc_base = inst_ptr,
		.pc_len = inst_len,
		.ipc = 0,
		.pc = 0
	};
	return frame;
}
*/
vm_reg_cd_t vm_get_desc(vima_t *vm)
{

}

void vm_set_desc(vima_t *vm, vm_reg_cd_t desc)
{

}


uint64_t vm_get_ipc(vima_t *vm)
{
	return vm->regs.code_desc.ipc;
}

uint64_t vm_get_pc(vima_t *vm)
{
	return vm->regs.code_desc.pc;

}

void vm_set_ipc(vima_t *vm, uint64_t set)
{
	vm->regs.code_desc.ipc = set;

}

void vm_set_pc(vima_t *vm, uint64_t set)
{
	vm->regs.code_desc.pc = set;

}

void vm_inc_ipc(vima_t *vm, uint64_t inc)
{
	vm->regs.code_desc.ipc+=inc;

}

void vm_inc_pc(vima_t *vm, uint64_t inc)
{

	vm->regs.code_desc.pc+=inc;
}


uint64_t vm_get_pc_base(vima_t *vm)
{
	return vm->regs.code_desc.pc_base;
}
uint64_t vm_get_ipc_base(vima_t *vm)
{
	return vm->regs.code_desc.ipc_base;

}

uint64_t vm_get_pc_len(vima_t *vm)
{
	return vm->regs.code_desc.pc_len;
}

uint64_t vm_get_ipc_len(vima_t *vm)
{
	return vm->regs.code_desc.ipc_len;

}


void vm_set_sfp(vima_t *vm,uint64_t set)
{
	vm->regs.stack.sfp = set;
}
uint64_t vm_get_sfp(vima_t *vm)
{
	return vm->regs.stack.sfp;

}
void vm_set_sp(vima_t *vm,uint64_t set)
{
	vm->regs.stack.sp = set;

}

uint64_t vm_get_sp(vima_t *vm)
{
	return vm->regs.stack.sp;
}


uint64_t vm_dec_sp(vima_t *vm, uint64_t degree)
{
	vm->regs.stack.sp -= degree;
	return vm->regs.stack.sp ;
}
uint64_t vm_inc_sp(vima_t *vm, uint64_t degree)
{
	uint64_t sp = vm->regs.stack.sp;
	vm->regs.stack.sp += degree;
	return sp;
}

/*
uint32_t vm_get_inst_at_pc_address(vima_t *vm, uint64_t address)
{
	uint64_t dest = (address / 2) + vm_get_pc_base(vm);
	if(address % 2 == 0)
	{
		return (uint32_t)((uint64_t)(vm_load(vm, dest) >> 32) & 0xffffffff);

	}
	else
	{
		return (uint32_t)((vm_load(vm, dest) & 0xffffffff));
	}
}
*/

uint32_t vm_get_inst_aligned_pc(int64_t val, bool even)
{
	if(even)
	{
		return (uint32_t)((uint64_t)(val >> 32) & 0xffffffff);

	}
	else
	{
		return (uint32_t)((val & 0xffffffff));
	}
}


//get the right address for memory
uint64_t vm_address(vima_t *vm, uint64_t addr)
{

}

int64_t vm_cpu_get_reg(vima_t *vm,int reg)
{
	if(reg == 0)
		return 0;
	return vm->regs.regs[reg];
}

//register set
void vm_cpu_set_reg(vima_t *vm, int reg, int64_t content)
{
	if(reg == 0)
		return;
	vm->regs.regs[reg] = content;

}

