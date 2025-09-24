#include "core.h"
#include "flags.h"
#include "palu.h"
#include "psys.h"
#include "pjmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

ldst_type memtype;

#define MEMLD(address) load(address)
#define CCPU(part) components.cpu->part
#define DROPCPU print_cpu_state(components.cpu);

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

uint64_t stream_in[255] = {0};
uint64_t stream_out[255] = {0};

uint64_t load(uint64_t address)
{
	if(memtype == ldst_fake)
	{
		return stream_in[address % (sizeof(stream_in) / sizeof(stream_in[0]))];
	}
	else
	{
		return memory_read(components.mem, address );
	}
}
void store(uint64_t address, int64_t value)
{
	if(memtype == ldst_fake)
	{
		stream_out[address % (sizeof(stream_out) / sizeof(stream_out[0]))] =  value;
	}
	else
	{
		memory_write(components.mem, address, value);
	}
}

void step_cpu(void)
{
	static int cycles = 0;
	if(CCPU(imm_pc_start) + CCPU(imm_pc_limit) < CCPU(imm_pc))
	{
		return;
	}
	if(CCPU(inst_pc_start) + CCPU(inst_pc_limit) < CCPU(inst_pc))
	{
		return;
	}

	switch(CCPU(stage))
	{
		case FETCH:
			fetch_cpu();
			break;
		case DECODE:
			decode_cpu();
		   	break;
		case EXECUTE:
			execute_cpu();
			break;
		case MEMORY:
			memory_cpu();
			break;
		case WRITEBACK:
			writeback_cpu();
			break;
	}

	if(CCPU(stage) == WRITEBACK)
	{
		//printf("%d\n", CCPU(stage));

		CCPU(stage) = FETCH;
	}
	else
	{
		CCPU(stage) += 1;
	}
	//DROPCPU;
	cycles++;
}

void startup_cpu(void)
{

	sys_call_cd_ptr(components.cpu, 0);
}


void fetch_cpu(void)
{
	CCPU(curins) = MEMLD(CCPU(inst_pc));
	CCPU(curimm) = MEMLD(CCPU(imm_pc));

}
void decode_cpu(void)
{
	inst_t inst = decode_inst(CCPU(curins));

	print_inst(&inst);
}
void execute_cpu(void)
{
	inst_t inst = decode_inst(CCPU(curins));

	if(inst.path == PATH_ALU)
	{
		alu_submit(components.alu, inst.subpath, get_reg(inst.rs1), get_reg(inst.rs2), CCPU(curimm), inst.immflag, inst.aux);
		alu_step(components.alu);
		while(!alu_poll(components.alu))
		{

			alu_step(components.alu);
		}
		CCPU(co) = components.alu->regdest;
	}
	if(inst.path == PATH_SYS)
	{
		system_submit(components.cpu);
	}
	if(inst.path == PATH_JMP)
	{
		jump_submit(components.cpu, inst.subpath, 0, get_reg(inst.rs1), get_reg(inst.rs2), CCPU(curimm), inst.immflag);


	}
	else
	{

	}
}
void memory_cpu(void)
{

}
void writeback_cpu(void)
{

	inst_t inst = decode_inst(CCPU(curins));
	printf("%d\n", CCPU(inst_pc));

	set_reg(inst.rd, CCPU(co));
	if(inst.path != PATH_JMP)
	{
		CCPU(inst_pc) ++;
		CCPU(imm_pc) += (inst.immflag)? 1:0;

	}


}




int64_t get_reg(int reg)
{
	if(reg >= 32 || reg <= 0)
	{
		return 0;
	}
	if(CCPU(mode) == KERNAL)
	{

		return CCPU(kregs)[reg];
	}
	else
	{
		return CCPU(regs)[reg];
	}
}


void set_reg(int reg, int64_t content)
{
	if(reg >= 32 || reg <= 0)
	{
		return;
	}
	if(CCPU(mode) == KERNAL)
	{

		CCPU(kregs)[reg] = content;
	}
	else
	{
		CCPU(regs)[reg] = content;
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
	in.aux = (instr >> 2) & 0x8;
	in.immflag = instr & 0x3;
	return in;
}

int32_t encode_inst(inst_t *inst)
{

	return ((inst->path << 28) | (inst->subpath << 20) | (inst->rd << 15) | (inst->rs1 << 10) | (inst->rs2 << 5) | (inst->aux << 2) | inst->immflag);

}

uint64_t encode(uint64_t path, uint64_t subpath, uint64_t rd, uint64_t rs1, uint64_t rs2, uint64_t aux, uint64_t immf)
{
	uint64_t inst = ( (path & 0xF) << 28) | ((subpath &0xff) << 20) | ((rd &0x1f) << 15) | ((rs1 &0x1f) << 10) | ((rs2 &0x1f) << 5) | ((aux & 0x8) << 2) | (immf);
	printf("%d\n", inst);
	print_bin(inst, 32, 1);
	return inst;
}

void print_inst(inst_t *inst)
{
	inst_t op = *inst;
	printf("p: %d: sp: %d, rd: %d, rs1: %d, rs2: %d, aux :%d, f: %d\n", op.path, op.subpath, op.rd, op.rs1, op.rs2, op.aux, op.immflag);

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


void print_cpu_state(cpu_t *cpu)
{

	printf("inst pc: %d, imm pc: %d\n", cpu->inst_pc, cpu->imm_pc);

	printf("kernal code description ptr = %d\n", cpu->k_cd_ptr);
	printf("user code description ptr = %d\n", cpu->u_cd_ptr);

}



void print_regs(void)
{
	for(int i = 0; i < 32; ++i)
	{
		printf("reg:%d = %d\n", i,	get_reg(i));

	}
}

void print_bin(long bin, char len, bool newline)
{
#define BUFSIZE 65
    char buffer[BUFSIZE] = {0};
    for (int i = 0, b = len - 1; i < len; i++, b--)
    {
        char bit = GETBIT(bin, b);
        buffer[i] = (char)(bit + '0');
    }
    printf("%s", buffer);
    if(newline)
    {
        printf("\n");
    }
}
void print_hex(char hex, bool newline)
{
    printf("0x%02hhx%c", hex, (newline)? '\n':0x00);
}

void print_str_hex(char *str, bool newline)
{

    for(char *temp = str; *temp != 0; temp++)
    {
        print_hex(*temp, false);
        if(*temp == '\n')
            printf("->'%s' ", "\\n");
        else
            printf("->'%c' ", *temp);
    }
    if(newline)
        printf("\n");

}


//Maybe return new length
void reverse(char *ary)
{
    size_t len = strlen(ary);
    char tstr[len + 1];
    memset(tstr, 0, len + 1);
    strcpy(tstr, ary);
    int initial = 0, end = len - 1;
    for (int i = initial; i < end; i++)
    {
        char temp = tstr[i];
        tstr[i] = tstr[end];
        tstr[end] = temp;
        end--;
    }
    strcpy(ary, tstr);

}
