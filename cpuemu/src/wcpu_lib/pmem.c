
#include "pmem.h"
#include "flags.h"
#include "cpumemory.h"
#include "core.h"
#include <stdio.h>

void memory_submit(cpu_t *cpu, char subpath, int64_t rs1, int64_t rs2, int64_t imm, char immflag )

{
	inst_t inst = decode_inst(cpu->curins);
	uint64_t address;
	if(immflag == 0)
		imm = 0;
	switch(inst.subpath)
	{
		case MEM_LD:
			address = rs1 + rs2 + imm;
			uint64_t ld = load(address);
			printf("load %d = %d\n", address, ld);
			cpu->co = ld;
			break;
		case MEM_SD:
			address = rs2 + imm;
			uint64_t sd = get_reg(rs1);
			printf("store [%d] = %d \n", address, sd );

			store(address, sd);
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

		default:
			printf("nothing done\n");
			break;
	}
}
