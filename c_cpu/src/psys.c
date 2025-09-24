
#include "psys.h"
#include "memory.h"
#include "core.h"


#define INST(S, M, A)  SYS_ ## S ## _ ## M ## _ ## A
#define CPU(item) cpu->item

void system_submit(cpu_t *cpu)
{

	switch(	decode_inst(cpu->curins).subpath)
	{
		case SYS_SET_CD_PTR:
			sys_set_cd_ptr(cpu, cpu->co);
			break;
		case SYS_CALL_CD_PTR:
			sys_call_cd_ptr(cpu, cpu->co);
			break;
	}
}


void sys_set_cd_ptr(cpu_t *cpu, uint64_t rd)
{
	if(cpu->mode == KERNAL)
	{
		cpu->k_cd_ptr = rd;

	}
	else
	{
		cpu->u_cd_ptr = rd;
	}
}

void sys_call_cd_ptr(cpu_t *cpu, uint64_t rd)
{

	if(cpu->mode == KERNAL)
	{
		uint64_t address =CPU(k_cd_ptr);
		int64_t cdtb_ptr = load(address);
		int64_t cdtb_len = load(address + 1);
		int64_t inst_ptr = load(address + 2);
		int64_t inst_len = load(address + 3);
		int64_t imm_ptr = load(address + 4);
		int64_t imm_len = load(address + 5);


		CPU(inst_pc) = inst_ptr;
		CPU(inst_pc_limit) = inst_len;
		CPU(imm_pc) = imm_ptr;
		CPU(imm_pc_limit) = imm_len;
		CPU(imm_pc_start) = imm_ptr;
		CPU(inst_pc_start) = inst_ptr;
		CPU(k_cd_ptr) = cdtb_ptr;
		CPU(k_cd_len) = cdtb_len;
	}
	else
	{
		uint64_t address =CPU(u_cd_ptr);
		int64_t cdtb_ptr = load(address);
		int64_t cdtb_len = load(address + 1);
		int64_t inst_ptr = load(address + 2);
		int64_t inst_len = load(address + 3);
		int64_t imm_ptr = load(address + 4);
		int64_t imm_len = load(address + 5);


		CPU(inst_pc) = inst_ptr;
		CPU(inst_pc_limit) = inst_len;
		CPU(imm_pc) = imm_ptr;
		CPU(imm_pc_limit) = imm_len;
		CPU(imm_pc_start) = imm_ptr;
		CPU(inst_pc_start) = inst_ptr;
		CPU(u_cd_ptr) = cdtb_ptr;
		CPU(u_cd_len) = cdtb_len;

	}

}

#undef CPU
#undef INST