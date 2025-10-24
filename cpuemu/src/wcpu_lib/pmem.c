
#include "pmem.h"
#include "flags.h"
#include "cpumemory.h"
#include "core.h"
#include <stdio.h>

void memory_submit(cpu_t *cpu)
{
	inst_t inst = decode_inst(cpu->curins);
	switch(inst.subpath)
	{
		case MEM_LD:
			uint64_t address = cpu->curimm + get_reg(inst.rs1) + get_reg(inst.rs2);
			uint64_t ld = load(address);
			printf("load %d %d %d\n", ld, address, ld);
			cpu->co = ld;
			break;
		case MEM_SD:
			store(get_reg(inst.rs1) + get_reg(inst.rs2) + cpu->curimm, get_reg(inst.rd));
			break;

		case MEM_SP:
			//dest to co
			cpu->co = get_sp();
			set_sp(get_reg(inst.rs1) + get_reg(inst.rs2) + cpu->curimm);

			break;
		case MEM_SFP:
			//dest to co
			cpu->co = get_sfp();
			set_sfp(get_reg(inst.rs1) + get_reg(inst.rs2) + cpu->curimm);

			break;

		default:
			printf("nothing done\n");
			break;
	}
}
