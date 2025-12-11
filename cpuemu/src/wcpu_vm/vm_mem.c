
#include "vm.h"
#include "vm_cpu.h"
#include "vm_mem.h"
#include "flags.h"
#include <stdint.h>
#include <stdio.h>


/*
void memory_submit(cpu_t *cpu, char subpath, int64_t lane1, int64_t lane2, int64_t lane3, char immflag )

{
	uint64_t address, sd, ld;

	switch(subpath)
	{
		case MEM_LDI:
			address = lane1 + lane2 + lane3;
			ld = load(address);
			//printf("load [[%d+%d+%d]=%d] = %d\n", lane1, lane2, lane3, address, ld);
			cpu->co = ld;
			break;
		case MEM_STI:
			address = lane2 + lane3;
			//printf("store [[%d+%d]=%d] = %d \n",lane2, lane3, address, lane1 );

			store(address, lane1);
			break;

		case MEM_SP:
			//dest to co
			cpu->co = get_sp();
			set_sp(lane1 + lane2 + lane3);

			break;
		case MEM_SFP:
			//dest to co
			cpu->co = get_sfp();
			set_sfp(lane1 + lane2 + lane3);

			break;
		case MEM_LDS:
			address = get_sp() + lane2 + lane3;
			ld = load(address);
			//printf("load %lld = %lld\n", address, ld);
			cpu->co = ld;
			break;

		case MEM_STS:
			address = get_sp() + lane2 + lane3;
			//printf("store [%lld] = %lld \n", address, lane1);

			store(address, lane1);
			break;
		case MEM_INCSP:

			inc_sp(lane2 + lane3);
			break;
		case MEM_DECSP:
			dec_sp(lane2 + lane3);

			break;
		case MEM_PUSH:

			address = inc_sp(1);

			store(address, lane1);

			break;

		case MEM_POP:
			address = dec_sp(1);
			cpu->co = load(address);
			break;
		default:
			//printf("nothing done\n");
			break;
	}
}
*/

void vm_cpu_path_mem_init(vima_t *vm)
{

}

void vm_cpu_path_mem_pull(vima_t *vm)
{

}
void vm_cpu_path_mem_push(vima_t *vm)
{

}
void vm_cpu_path_mem_exec(vima_t *vm)
{

}