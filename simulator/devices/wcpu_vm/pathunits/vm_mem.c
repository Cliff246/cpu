
/*
#include "vm.h"
#include "vm_cpu.h"
#include "vm_mem.h"
#include "vm_cpu_utils.h"
#include "flags.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>





void vm_cpu_path_mem_init(vima_t *vm)
{

}

int64_t vm_cpu_path_mem_exec(vima_t *vm, char subflag, int64_t lane1, int64_t lane2, int64_t lane3, bool swap)
{
	uint64_t address, sd, ld;
	int64_t result = 0;
	switch(subflag)
	{
		case MEM_LDI:
			address = lane1 + lane2 + lane3;
			ld = vm_load(vm, address);
			//printf("load [[%d+%d+%d]=%d] = %d\n", lane1, lane2, lane3, address, ld);
			result = ld;
			break;
		case MEM_STI:
			address = lane2 + lane3;
			//printf("store [[%d+%d]=%d] = %d \n",lane2, lane3, address, lane1 );

			vm_store(vm, address, lane1);
			break;

		case MEM_SP:
			//dest to co
			result = vm_get_sp(vm);
			vm_set_sp(vm,lane1 + lane2 + lane3);

			break;
		case MEM_SFP:
			//dest to co
			result = vm_get_sfp(vm);
			vm_set_sfp(vm,lane1 + lane2 + lane3);

			break;
		case MEM_LDS:
			address = vm_get_sp(vm) + lane2 + lane3;
			ld = vm_load(vm, address);
			//printf("load %lld = %lld\n", address, ld);
			result = ld;
			break;

		case MEM_STS:
			address = vm_get_sp(vm) + lane2 + lane3;
			//printf("store [%lld] = %lld \n", address, lane1);

			vm_store(vm, address, lane1);
			break;
		case MEM_INCSP:

			vm_inc_sp(vm,lane2 + lane3);
			break;
		case MEM_DECSP:
			vm_dec_sp(vm,lane2 + lane3);

			break;
		case MEM_PUSH:

			address = vm_inc_sp(vm,1);

			vm_store(vm, address, lane1);

			break;

		case MEM_POP:
			address = vm_dec_sp(vm,1);
			result = vm_load(vm, address);
			break;
		default:
			//printf("nothing done\n");
			break;
	}
	return result;
}

*/
