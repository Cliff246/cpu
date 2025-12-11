

#include "vm.h"
#include "vm_cpu.h"
#include "vm_cpu_utils.h"
#include "flags.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>

/*

inst_t get_inst_from_op(operation_t *op)
{
	inst_t inst;
	if(op->inst.decoded == true)
	{
		inst = op->inst.inst;
	}
	else
	{
		inst = decode_inst((uint32_t)op->inst.raw);
	}
	return inst;

}



operation_t fill_operation(uint32_t inst, int64_t imm)
{
	operation_t op = {0};
	op.inst.decoded = false;
	op.inst.inst = decode_inst(inst);
	op.inst.raw = inst;
   	op.imm.type = IMM_UNKNOWN;
	op.imm.imm = imm;
	//printf("filled %d imm %lld\n", inst, imm);
	return op;
}


void decode_operation(operation_t *op)
{
	if(op->inst.decoded)
	{
		return;
	}
	inst_t inst = decode_inst(op->inst.raw);
	op->inst.inst = inst;
	op->imm.type = (inst.immflag)? IMM_TRUE : IMM_FALSE;

}


reg_file_t get_current_file(cpu_t *cpu)
{

	if(cpu->mode == USER)
	{
		return cpu->user;
	}
	else
	{
		return cpu->kernal;
	}

}

reg_file_t get_mode_file(cpu_t *cpu, pmode_t mode)
{
	if(mode == USER)
	{
		return cpu->user;
	}
	else
	{
		return cpu->kernal;
	}
}
void set_current_file(cpu_t *cpu, reg_file_t file)
{

	if(cpu->mode == USER)
	{
		cpu->user = file;
	}
	else
	{
		cpu->kernal = file;
	}
}

void set_mode_file(cpu_t *cpu, reg_file_t file, pmode_t mode)
{
	if(mode == USER)
	{
		cpu->user = file;
	}
	else
	{
		cpu->kernal = file;
	}
}

void push_scd(cpu_t *cpu, code_desc_t desc)
{

	reg_file_t file = get_current_file(cpu);

	uint64_t stack_cd =  file.stack.scd;
	stack_cd -= 9;

	store(stack_cd, desc.cd_ptr);
	store(stack_cd + 1, desc.ct_ptr);
	store(stack_cd + 2, desc.ct_len);
	store(stack_cd + 3, desc.pc_ptr);
	store(stack_cd + 4, desc.pc_len);
	store(stack_cd + 5, desc.ipc_ptr);
	store(stack_cd + 6, desc.ipc_len);
	store(stack_cd + 7, desc.pc);
	store(stack_cd + 8, desc.ipc);

	file.stack.scd = stack_cd;
	set_current_file(cpu,file);
}

code_desc_t pop_scd(cpu_t *cpu)
{
	reg_file_t file = get_current_file(cpu);

	uint64_t stack_cd =  file.stack.scd;



	code_desc_t desc = {
		.cd_ptr = load(stack_cd),
		.ct_ptr = load(stack_cd + 1),
		.ct_len = load(stack_cd + 2),
		.pc_ptr = load(stack_cd + 3),
		.pc_len = load(stack_cd + 4),
		.ipc_ptr = load(stack_cd + 5),
		.ipc_len = load(stack_cd +6),
		.pc = load(stack_cd + 7),
		.ipc = load(stack_cd + 8)

	};

	file.stack.scd += 9;
	set_current_file(cpu, file);
	return desc;
}



code_desc_t get_desc_from_address(cpu_t *cpu, uint64_t address)
{
	int64_t cdtb_ptr = load(address);
	int64_t cdtb_len = load(address + 1);
	int64_t inst_ptr = load(address + 2);
	int64_t inst_len = load(address + 3);
	int64_t imm_ptr = load(address + 4);
	int64_t imm_len = load(address + 5);
	//set_pc(inst_ptr);
	//set_ipc(imm_ptr);
	code_desc_t frame = {
		.cd_ptr = address,
		.ct_ptr = cdtb_ptr,
		.ct_len = cdtb_len,
		.ipc_ptr = imm_ptr,
		.ipc_len = imm_len,
		.pc_ptr = inst_ptr,
		.pc_len = inst_len,
		.ipc = 0,
		.pc = 0
	};
	return frame;
}


uint64_t get_ipc(void)
{
	return (CCPU(mode) == KERNAL) ? CCPU(kernal.desc.ipc) : CCPU(user.desc.ipc);
}
uint64_t get_pc(void)
{
	return (CCPU(mode) == KERNAL) ? CCPU(kernal.desc.pc) : CCPU(user.desc.pc);
}

void set_ipc(uint64_t set)
{
	if (CCPU(mode) == KERNAL)
		CCPU(kernal.desc.ipc) = set;
	else
		CCPU(user.desc.ipc) = set;
}

void set_pc(uint64_t set)
{
	if (CCPU(mode) == KERNAL)
		CCPU(kernal.desc.pc) = set;
	else
		CCPU(user.desc.pc) = set;
	// printf("%d should be | is %d\n", set, CCPU(k_pc));
}

void inc_ipc(uint64_t inc)
{
	if (CCPU(mode) == KERNAL)
		CCPU(kernal.desc.ipc) += inc;
	else
		CCPU(user.desc.ipc) += inc;
}

void inc_pc(uint64_t inc)
{
	if (CCPU(mode) == KERNAL)
		CCPU(kernal.desc.pc) += inc;
	else
		CCPU(user.desc.pc) += inc;
}

uint64_t get_pc_offset(void)
{
	return (CCPU(mode) == KERNAL) ? CCPU(kernal.desc.pc_ptr) : CCPU(user.desc.pc_ptr);
}
uint64_t get_ipc_offset(void)
{

	return (CCPU(mode) == KERNAL) ? CCPU(kernal.desc.ipc_ptr) : CCPU(user.desc.ipc_ptr);
}

uint64_t get_pc_len(void)
{

	return (CCPU(mode) == KERNAL) ? CCPU(kernal.desc.pc_len) : CCPU(user.desc.pc_len);
}
uint64_t get_ipc_len(void)
{

	return (CCPU(mode) == KERNAL) ? CCPU(kernal.desc.ipc_len) : CCPU(user.desc.ipc_len);
}

uint64_t get_sp(void)
{
	return (CCPU(mode) == KERNAL) ? CCPU(kernal.stack.sp) : CCPU(user.stack.sp);
}

void set_sp(uint64_t set)
{
	if (CCPU(mode) == KERNAL)
		CCPU(kernal.stack.sp) = set;
	else
		CCPU(user.stack.sp) = set;
}

uint64_t get_sfp(void)
{
	return (CCPU(mode) == KERNAL) ? CCPU(kernal.stack.sf) : CCPU(user.stack.sf);
}

void set_sfp(uint64_t set)
{
	if (CCPU(mode) == KERNAL)
		CCPU(kernal.stack.sf) = set;
	else
		CCPU(user.stack.sf) = set;
}

uint64_t dec_sp(uint64_t degree)
{
	uint64_t val;
	if (CCPU(mode) == KERNAL)
	{
		(CCPU(kernal.stack.sp) -= degree);
		val = CCPU(kernal.stack.sp);

	}
	else
	{
		(CCPU(user.stack.sp) -= degree);
		val = CCPU(kernal.stack.sp);

	}
	return val;
}
uint64_t inc_sp(uint64_t degree)
{
	uint64_t val;
	if (CCPU(mode) == KERNAL)
	{
		val = CCPU(kernal.stack.sp);
		(CCPU(kernal.stack.sp) += degree);
	}
	else
	{
		val = CCPU(user.stack.sp);
		(CCPU(user.stack.sp) += degree);
	}
	return val;
}

code_desc_t get_desc(pmode_t mode)
{
	return (mode == KERNAL) ? CCPU(kernal.desc) : CCPU(user.desc);
}

void set_desc(pmode_t mode, code_desc_t frame)
{
	if (mode == KERNAL)
	{
		CCPU(kernal.desc) = frame;
	}
	else
	{
		CCPU(user.desc) = frame;
	}
}

int64_t get_reg(int reg)
{
	//printf("get reg %d\n", reg);
	if (reg >= 64 || reg <= 0)
	{
		return 0;
	}
	if (CCPU(mode) == KERNAL)
	{

		return CCPU(kernal.iregs)[reg];
	}
	else
	{
		return CCPU(user.iregs)[reg];
	}
}

void set_reg(int reg, int64_t content)
{
	//printf("set reg %d %d\n", reg, content);
	if (reg >= 64 || reg <= 0)
	{
		return;
	}
	if (CCPU(mode) == KERNAL)
	{

		CCPU(kernal.iregs)
		[reg] = content;
	}
	else
	{
		CCPU(user.iregs)
		[reg] = content;
	}
}


uint32_t get_inst_at_pc_address(uint64_t address)
{



	uint64_t dest = (address / 2) + get_pc_offset();
	if(address % 2 == 0)
	{
		return (uint32_t)((uint64_t)(MEMLD(dest) >> 32) & 0xffffffff);

	}
	else
	{
		return (uint32_t)((MEMLD(dest) & 0xffffffff));
	}
}

*/


int64_t vm_cpu_get_lane1(vima_t *vm)
{
	return vm->cpu.pre.lane1;
}

int64_t vm_cpu_get_lane2(vima_t *vm)
{
	return vm->cpu.pre.lane2;

}

int64_t vm_cpu_get_lane3(vima_t *vm)
{
	return vm->cpu.pre.lane3;

}


uint64_t vm_get_ipc(vima_t *vm)
{
	return vm->cpu.user.code_desc.ipc;
}

uint64_t vm_get_pc(vima_t *vm)
{
	return vm->cpu.user.code_desc.pc;

}

void vm_set_ipc(vima_t *vm, uint64_t set)
{
	vm->cpu.user.code_desc.ipc = set;

}

void vm_set_pc(vima_t *vm, uint64_t set)
{
	vm->cpu.user.code_desc.pc = set;

}

void vm_inc_ipc(vima_t *vm, uint64_t inc)
{
	vm->cpu.user.code_desc.ipc+=inc;

}

void vm_inc_pc(vima_t *vm, uint64_t inc)
{

	vm->cpu.user.code_desc.pc+=inc;
}


uint64_t vm_get_pc_base(vima_t *vm)
{
	return vm->cpu.user.code_desc.pc_base;
}
uint64_t vm_get_ipc_base(vima_t *vm)
{
	return vm->cpu.user.code_desc.ipc_base;

}

uint64_t vm_get_pc_len(vima_t *vm)
{
	return vm->cpu.user.code_desc.pc_len;
}

uint64_t vm_get_ipc_len(vima_t *vm)
{
	return vm->cpu.user.code_desc.ipc_len;

}


void vm_set_sfp(vima_t *vm,uint64_t set)
{
	vm->cpu.user.stack.sfp = set;
}
uint64_t vm_get_sfp(vima_t *vm)
{
	return vm->cpu.user.stack.sfp;

}
void vm_set_sp(vima_t *vm,uint64_t set)
{
	vm->cpu.user.stack.sp = set;

}

uint64_t vm_get_sp(vima_t *vm)
{
	return vm->cpu.user.stack.sp;
}


uint64_t vm_dec_sp(vima_t *vm, uint64_t degree)
{
	vm->cpu.user.stack.sp -= degree;
	return vm->cpu.user.stack.sp;
}
uint64_t vm_inc_sp(vima_t *vm, uint64_t degree)
{
	uint64_t sp = vm->cpu.user.stack.sp;
	vm->cpu.user.stack.sp += degree;
	return sp;
}

uint32_t vm_get_inst_at_pc_address(vima_t *vm,uint64_t address)
{

}

//get the right address for memory
uint64_t vm_address(vima_t *vm,uint64_t addr)
{

}

int64_t vm_cpu_get_reg(vima_t *vm,int reg)
{
	return vm->cpu.user.regs[reg];
}

//register set
void vm_cpu_set_reg(vima_t *vm, int reg, int64_t content)
{
	vm->cpu.user.regs[reg] = content;

}
