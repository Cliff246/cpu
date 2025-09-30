
#include "psys.h"
#include "cpumemory.h"
#include "core.h"
#include "coreutils.h"
#include <stdio.h>


#define INST(S, M, A)  SYS_ ## S ## _ ## M ## _ ## A
#define CPU(item) cpu->item

void system_submit(cpu_t *cpu)
{

	switch(decode_inst(cpu->curins).subpath)
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
		cpu->k_cd.cd_ptr = rd;

	}
	else
	{
		cpu->k_cd.cd_ptr = rd;
	}
}

void sys_call_cd_ptr(cpu_t *cpu, uint64_t rd)
{

	if(cpu->mode == KERNAL)
	{
		uint64_t address = CPU(u_cd.cd_ptr);
		cd_frame_t frame = get_frame_from_address(cpu, rd);
		set_frame(KERNAL, frame);
	}
	else
	{
		uint64_t address = CPU(u_cd.cd_ptr);
		cd_frame_t frame = get_frame_from_address(cpu, rd);
		set_frame(USER, frame);

	}

}

#undef CPU
#undef INST
