
#include "psys.h"
#include "cpumemory.h"
#include "core.h"
#include "coreutils.h"
#include <stdio.h>
#include <coreutils.h>


#define INST(S, M, A)  SYS_ ## S ## _ ## M ## _ ## A
#define CPU(item) cpu->item

void system_submit(cpu_t *cpu)
{
	inst_t inst = get_inst_from_op(&cpu->op);
	switch(inst.subpath)
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

	reg_file_t file = get_current_file(cpu);
	
	file.desc.cd_ptr = rd;
	set_current_file(cpu, file);
}

void sys_call_cd_ptr(cpu_t *cpu, uint64_t rd)
{

	reg_file_t file =  get_current_file(cpu);

	uint64_t address = file.desc.cd_ptr;
	code_desc_t frame = get_desc_from_address(cpu, rd);
	set_desc(cpu->mode, frame);


}

#undef CPU
#undef INST
