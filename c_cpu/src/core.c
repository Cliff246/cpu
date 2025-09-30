#include "core.h"
#include "flags.h"
#include "palu.h"
#include "psys.h"

#include "pjmp.h"
#include "debug.h"
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

uint64_t get_ipc(void)
{
	return (CCPU(mode) == KERNAL)? CCPU(k_cd.ipc) : CCPU(u_cd.ipc);
}
uint64_t get_pc(void)
{
	return (CCPU(mode) == KERNAL)? CCPU(k_cd.pc) : CCPU(u_cd.pc);
}

void set_ipc(uint64_t set)
{
	if(CCPU(mode) == KERNAL)
		CCPU(k_cd.ipc) = set;
	else
		CCPU(u_cd.ipc) = set;
}

void set_pc(uint64_t set)
{
	if(CCPU(mode) == KERNAL)
		CCPU(k_cd.pc) = set;
	else
		CCPU(u_cd.ipc) = set;
	//printf("%d should be | is %d\n", set, CCPU(k_pc));
}

void inc_ipc(uint64_t inc)
{
	if(CCPU(mode) == KERNAL)
		CCPU(k_cd.ipc) += inc;
	else
		CCPU(u_cd.ipc) += inc;
}

void inc_pc(uint64_t inc)
{
	if(CCPU(mode) == KERNAL)
		CCPU(k_cd.pc) += inc;
	else
		CCPU(u_cd.pc) += inc;

}

uint64_t get_pc_offset(void)
{
	return (CCPU(mode) == KERNAL)? CCPU(k_cd.pc_ptr) : CCPU(u_cd.pc_ptr);

}
uint64_t get_ipc_offset(void)
{

	return (CCPU(mode) == KERNAL)? CCPU(k_cd.ipc_ptr) : CCPU(u_cd.ipc_ptr);
}

uint64_t get_pc_len(void)
{

	return (CCPU(mode) == KERNAL)? CCPU(k_cd.pc_len) : CCPU(u_cd.pc_len);
}
uint64_t get_ipc_len(void)
{

	return (CCPU(mode) == KERNAL)? CCPU(k_cd.ipc_len) : CCPU(u_cd.ipc_len);
}



cd_frame_t get_frame(pmode_t mode)
{
	return (mode == KERNAL)? CCPU(k_cd) : CCPU(u_cd);
}

void set_frame(pmode_t mode, cd_frame_t frame)
{
	if(mode == KERNAL)
	{
		CCPU(k_cd) = frame;
	}
	else
	{
		CCPU(u_cd) = frame;
	}
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

	//printf("stage: %d\n", CCPU(stage));

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
	CCPU(stage) = 0;
	sys_call_cd_ptr(components.cpu, 0);

	CCPU(stage) = 0;
}


void fetch_cpu(void)
{
	uint64_t pc = get_pc() + get_pc_offset();
	uint64_t ipc = get_ipc() + get_ipc_offset();
#if DEBUG_MODE == 1
	printf("pc: %llu ipc: %llu\n", pc, ipc);
	printf("fpc: %llu fipc: %llu\n", get_pc(), get_ipc());
#endif
	CCPU(curins) = MEMLD(pc);
	CCPU(curimm) = MEMLD(ipc);
}
void decode_cpu(void)
{
	inst_t inst = decode_inst(CCPU(curins));
#if DEBUG_MODE == 1
	print_inst(&inst);
#endif
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

		set_reg(inst.rd, CCPU(co));
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
	if(inst.path != PATH_JMP)
	{

		inc_pc(1);
		inc_ipc((inst.immflag)? 1:0);
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
	in.subpath = (instr >> 21) & 0x7F;
	in.rd = (instr >> 16) & 0x1F;
	in.rs1 = (instr >> 11) & 0x1F;
	in.rs2 = (instr >> 6) & 0x1F;
	in.aux = (instr >> 2) & 0xF;
	in.immflag = instr & 0x3;
	return in;
}

int32_t encode_inst(inst_t *inst)
{

	return ((inst->path << 28) | (inst->subpath << 21) | (inst->rd << 16) | (inst->rs1 << 11) | (inst->rs2 << 6) | (inst->aux << 2) | inst->immflag);

}

uint64_t encode(uint64_t path, uint64_t subpath, uint64_t rd, uint64_t rs1, uint64_t rs2, uint64_t aux, uint64_t immf)
{
	uint64_t inst = ( (path & 0xF) << 28) | ((subpath &0x7F) << 21) | ((rd &0x1f) << 16) | ((rs1 &0x1f) << 11) | ((rs2 &0x1f) << 6) | ((aux & 0xf) << 2) | (immf);
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

	printf("inst pc: %llu, imm pc: %llu\n", get_pc(), get_ipc());

}



void print_regs(void)
{
	for(int i = 0; i < 32; ++i)
	{
		printf("reg:%d = %d\n", i,	get_reg(i));

	}
}

