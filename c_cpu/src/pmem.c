
#include "pmem.h"
#include "flags.h"
#include "memory.h"
#include "core.h"

void memory_submit(cpu_t *cpu)
{
	inst_t inst = decode_inst(cpu->curins);
	switch(inst.subpath)
	{
		case MEM_LD:
			set_reg(inst.rd, load(cpu->curimm + get_reg(inst.rs1) + get_reg(inst.rs2)));
			break;
		case MEM_SD:
			store(get_reg(inst.rs1) + get_reg(inst.rs2) + cpu->curimm, get_reg(inst.rd)); 
			break;
	}
}
