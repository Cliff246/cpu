//
#include "vm_sys.h"
#include "vima.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
/*

#define INST(S, M, A)  SYS_ ## S ## _ ## M ## _ ## A
#define CPU(item) cpu->item

void system_submit(vima_t *cpu)
{
	vm_inst_t inst = g
	switch(inst.subpath)
	{
		case SYS_SET_CD_PTR:
			sys_set_cd_ptr(cpu, cpu->co);
			break;
		case SYS_CALL_CD_PTR:
			sys_call_cd_ptr(cpu, cpu->co);
			break;
		case SYS_BREAK:
			cpu->stop = true;
			break;
	}
}


void sys_set_cd_ptr(vima_t *cpu, uint64_t rd)
{

	reg_file_t file = get_current_file(cpu);

	file.desc.cd_ptr = rd;
	set_current_file(cpu, file);
}

void sys_call_cd_ptr(vima_t *cpu, uint64_t rd)
{

	reg_file_t file =  get_current_file(cpu);

	uint64_t address = file.desc.cd_ptr;
	code_desc_t frame = get_desc_from_address(cpu, rd);
	set_desc(cpu->mode, frame);


}

#undef CPU
#undef INST
*/

void vm_cpu_path_sys_init(vima_t *vm)
{

}

int64_t vm_cpu_path_sys_exec(vima_t *vm, char subflag, int64_t lane1, int64_t lane2, int64_t lane3, bool swap)
{


	switch(subflag)
	{
		case SYS_SET_CD_PTR:
			break;
		case SYS_CALL_CD_PTR:
			break;


			
		case SYS_BREAK:
			break;
	}

	return 0;
}
