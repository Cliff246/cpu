
#include "pmem.h"
#include "flags.h"
#include "cpumemory.h"
#include "core.h"
#include <coreutils.h>
#include <stdio.h>

void memory_submit(cpu_t *cpu, char subpath, int64_t rs1, int64_t rs2, int64_t imm, char immflag )

{
	uint64_t address, sd, ld;
	if(immflag == 0)
		imm = 0;
	switch(subpath)
	{
		case MEM_LD:
			address = rs1 + rs2 + imm;
			ld = load(address);
			printf("load %d = %d\n", address, ld);
			cpu->co = ld;
			break;
		case MEM_SD:
			address = rs2 + imm;
			printf("store [%d] = %d \n", address, rs1 );

			store(address, rs1);
			break;

		case MEM_SP:
			//dest to co
			cpu->co = get_sp();
			set_sp(rs1 + rs2 + imm);

			break;
		case MEM_SFP:
			//dest to co
			cpu->co = get_sfp();
			set_sfp(rs1 + rs2 + imm);

			break;
		case MEM_LDS:
			address = get_sp()  + rs2 + imm;
			ld = load(address);
			printf("load %lld = %lld\n", address, ld);
			cpu->co = ld;
			break;

		case MEM_SDS:
			address = get_sp() + rs2 + imm;
			printf("store [%lld] = %lld \n", address, rs1);

			store(address, rs1);
			break;
		case MEM_INCSP:

			inc_sp(rs2 + imm);
			break;
		case MEM_DECSP:
			dec_sp(rs2 + imm);

			break;
		case MEM_PUSH:

			address = inc_sp(1);

			store(address, rs1);

			break;

		case MEM_POP:
			address = dec_sp(1);
			cpu->co = load(address);
			break;
		default:
			printf("nothing done\n");
			break;
	}
}
