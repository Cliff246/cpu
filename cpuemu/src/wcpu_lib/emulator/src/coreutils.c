

#include "coreutils.h"
#include "flags.h"
#include <string.h>

#define CCPU(part) components.cpu->part
#define MEMLD(address) load(address)


void push_scd(cpu_t *cpu, cd_frame_t frame)
{
	uint64_t stack_cd = (cpu->mode == KERNAL)? cpu->k_stack.scd: cpu->u_stack.scd;
	stack_cd -= 9;

	store(stack_cd, frame.cd_ptr);
	store(stack_cd + 1, frame.ct_ptr);
	store(stack_cd + 2, frame.ct_len);
	store(stack_cd + 3, frame.pc_ptr);
	store(stack_cd + 4, frame.pc_len);
	store(stack_cd + 5, frame.ipc_ptr);
	store(stack_cd + 6, frame.ipc_len);
	store(stack_cd + 7, frame.pc);
	store(stack_cd + 8, frame.ipc);


	if(cpu->mode == KERNAL)
		cpu->k_stack.scd = stack_cd;
	else
		cpu->u_stack.scd = stack_cd;
}

cd_frame_t pop_scd(cpu_t *cpu)
{
	uint64_t stack_cd = (cpu->mode == KERNAL)? cpu->k_stack.scd: cpu->u_stack.scd;



	cd_frame_t frame = {
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

	if(cpu->mode == KERNAL)
		cpu->k_stack.scd = stack_cd + 9;
	else
		cpu->u_stack.scd = stack_cd + 9;
	return frame;
}
cd_frame_t get_frame_from_address(cpu_t *cpu, uint64_t address)
{
	int64_t cdtb_ptr = load(address);
	int64_t cdtb_len = load(address + 1);
	int64_t inst_ptr = load(address + 2);
	int64_t inst_len = load(address + 3);
	int64_t imm_ptr = load(address + 4);
	int64_t imm_len = load(address + 5);
	//set_pc(inst_ptr);
	//set_ipc(imm_ptr);
	cd_frame_t frame = {
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
	return (CCPU(mode) == KERNAL) ? CCPU(k_cd.ipc) : CCPU(u_cd.ipc);
}
uint64_t get_pc(void)
{
	return (CCPU(mode) == KERNAL) ? CCPU(k_cd.pc) : CCPU(u_cd.pc);
}

void set_ipc(uint64_t set)
{
	if (CCPU(mode) == KERNAL)
		CCPU(k_cd.ipc) = set;
	else
		CCPU(u_cd.ipc) = set;
}

void set_pc(uint64_t set)
{
	if (CCPU(mode) == KERNAL)
		CCPU(k_cd.pc) = set;
	else
		CCPU(u_cd.pc) = set;
	// printf("%d should be | is %d\n", set, CCPU(k_pc));
}

void inc_ipc(uint64_t inc)
{
	if (CCPU(mode) == KERNAL)
		CCPU(k_cd.ipc) += inc;
	else
		CCPU(u_cd.ipc) += inc;
}

void inc_pc(uint64_t inc)
{
	if (CCPU(mode) == KERNAL)
		CCPU(k_cd.pc) += inc;
	else
		CCPU(u_cd.pc) += inc;
}

uint64_t get_pc_offset(void)
{
	return (CCPU(mode) == KERNAL) ? CCPU(k_cd.pc_ptr) : CCPU(u_cd.pc_ptr);
}
uint64_t get_ipc_offset(void)
{

	return (CCPU(mode) == KERNAL) ? CCPU(k_cd.ipc_ptr) : CCPU(u_cd.ipc_ptr);
}

uint64_t get_pc_len(void)
{

	return (CCPU(mode) == KERNAL) ? CCPU(k_cd.pc_len) : CCPU(u_cd.pc_len);
}
uint64_t get_ipc_len(void)
{

	return (CCPU(mode) == KERNAL) ? CCPU(k_cd.ipc_len) : CCPU(u_cd.ipc_len);
}

uint64_t get_sp(void)
{
	return (CCPU(mode) == KERNAL) ? CCPU(k_stack.sp) : CCPU(u_stack.sp);
}

void set_sp(uint64_t set)
{
	if (CCPU(mode) == KERNAL)
		CCPU(k_stack.sp) = set;
	else
		CCPU(u_stack.sp) = set;
}

uint64_t get_sfp(void)
{
	return (CCPU(mode) == KERNAL) ? CCPU(k_stack.sf) : CCPU(u_stack.sf);
}

void set_sfp(uint64_t set)
{
	if (CCPU(mode) == KERNAL)
		CCPU(k_stack.sf) = set;
	else
		CCPU(u_stack.sf) = set;
}

uint64_t dec_sp(uint64_t degree)
{
	uint64_t val;
	if (CCPU(mode) == KERNAL)
		val = (CCPU(k_stack.sp) -= degree);
	else
		val = (CCPU(u_stack.sp) -= degree);
	return val;
}
uint64_t inc_sp(uint64_t degree)
{
	uint64_t val;

	if (CCPU(mode) == KERNAL)
		val = (CCPU(k_stack.sp) += degree);
	else
		val = (CCPU(u_stack.sp) += degree);
	return val;
}

cd_frame_t get_frame(pmode_t mode)
{
	return (mode == KERNAL) ? CCPU(k_cd) : CCPU(u_cd);
}

void set_frame(pmode_t mode, cd_frame_t frame)
{
	if (mode == KERNAL)
	{
		CCPU(k_cd) = frame;
	}
	else
	{
		CCPU(u_cd) = frame;
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

		return CCPU(kregs)[reg];
	}
	else
	{
		return CCPU(regs)[reg];
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

		CCPU(kregs)
		[reg] = content;
	}
	else
	{
		CCPU(regs)
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

