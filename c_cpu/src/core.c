#include "core.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>



components_t components; 

cpu_t *create_cpu(void)
{
	cpu_t *ptr = (cpu_t *)calloc(1 ,sizeof(cpu_t));
	if(!ptr)
	{
		errno = ENOMEM;
		return NULL;
	}
	return ptr;
}


void step_cpu()
{
	static int count = 0;
	
	cpu_t *cpu = components.cpu;	
	printf("%d\n", count);
	count++;		
}



int64_t get_reg(int reg)
{
	if(reg >= 32 || reg <= 0)
	{
		return 0;
	}	
	else
	{
		return components.cpu->regs[reg];
	}	
}


void set_reg(int reg, int64_t content)
{
	if(reg >= 32 || reg <= 0)
	{
		return;
	}	
	else
	{
		components.cpu->regs[reg] = content;
		return;
	}	
	
}



inst_t decode_inst(int32_t instr)
{
	inst_t in;
	in.path = (instr >> 28) & 0xF;
	in.subpath = (instr >> 20) & 0xFF;
	in.rd = (instr >> 15) & 0x1F;
	in.rs1 = (instr >> 10) & 0x1F;
	in.rs2 = (instr >> 5) & 0x1F;
	in.aux = (instr >> 2) & 0xF;
	in.immflag = instr & 0x2;
	return in;
}



void init_components(void)
{
	
	cpu_t *cpu = create_cpu();
	
	memory_t *mem = create_memory(1000);
	alu_t *alu = create_alu();

	components.cpu = cpu;
	components.mem = mem;
	components.alu = alu;
	

}

void free_components(void)
{
	free(components.cpu);
	free(components.mem->content);
	free(components.mem);
	free(components.alu);
	components.cpu = NULL;
	components.mem = NULL;
	components.alu = NULL;
}
