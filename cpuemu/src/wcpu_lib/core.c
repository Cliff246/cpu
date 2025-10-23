#include "core.h"
#include "flags.h"
#include "palu.h"
#include "psys.h"
#include "cpumemory.h"

#include "pjmp.h"
#include "debug.h"
#include "pmem.h"
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
	cpu_t *ptr = (cpu_t *)calloc(1, sizeof(cpu_t));
	if (!ptr)
	{
		errno = ENOMEM;
		return NULL;
	}
	return ptr;
}

uint64_t get_ipc(void)
{
	return (CCPU(mode) == KERNAL) ? CCPU(k_cd.ipc) : CCPU(u_cd.ipc);
}
uint64_t get_pc(void)
{
	return (CCPU(mode) == KERNAL) ? CCPU(k_cd.pc) : CCPU(u_cd.pc);
}

void set_ipc(uint64_t set)
{
	if (CCPU(mode) == KERNAL)
		CCPU(k_cd.ipc) = set;
	else
		CCPU(u_cd.ipc) = set;
}

void set_pc(uint64_t set)
{
	if (CCPU(mode) == KERNAL)
		CCPU(k_cd.pc) = set;
	else
		CCPU(u_cd.pc) = set;
	// printf("%d should be | is %d\n", set, CCPU(k_pc));
}

void inc_ipc(uint64_t inc)
{
	if (CCPU(mode) == KERNAL)
		CCPU(k_cd.ipc) += inc;
	else
		CCPU(u_cd.ipc) += inc;
}

void inc_pc(uint64_t inc)
{
	if (CCPU(mode) == KERNAL)
		CCPU(k_cd.pc) += inc;
	else
		CCPU(u_cd.pc) += inc;
}

uint64_t get_pc_offset(void)
{
	return (CCPU(mode) == KERNAL) ? CCPU(k_cd.pc_ptr) : CCPU(u_cd.pc_ptr);
}
uint64_t get_ipc_offset(void)
{

	return (CCPU(mode) == KERNAL) ? CCPU(k_cd.ipc_ptr) : CCPU(u_cd.ipc_ptr);
}

uint64_t get_pc_len(void)
{

	return (CCPU(mode) == KERNAL) ? CCPU(k_cd.pc_len) : CCPU(u_cd.pc_len);
}
uint64_t get_ipc_len(void)
{

	return (CCPU(mode) == KERNAL) ? CCPU(k_cd.ipc_len) : CCPU(u_cd.ipc_len);
}

uint64_t get_sp(void)
{
	return (CCPU(mode) == KERNAL) ? CCPU(k_stack.sp) : CCPU(u_stack.sp);
}

void set_sp(uint64_t set)
{
	if (CCPU(mode) == KERNAL)
		CCPU(k_stack.sp) = set;
	else
		CCPU(u_stack.sp) = set;
}

uint64_t get_sfp(void)
{
	return (CCPU(mode) == KERNAL) ? CCPU(k_stack.sf) : CCPU(u_stack.sf);
}

void set_sfp(uint64_t set)
{
	if (CCPU(mode) == KERNAL)
		CCPU(k_stack.sf) = set;
	else
		CCPU(u_stack.sf) = set;
}

uint64_t dec_sp(uint64_t degree)
{
	uint64_t val;
	if (CCPU(mode) == KERNAL)
		val = (CCPU(k_stack.sp) -= degree);
	else
		val = (CCPU(u_stack.sp) -= degree);
	return val;
}
uint64_t inc_sp(uint64_t degree)
{
	uint64_t val;

	if (CCPU(mode) == KERNAL)
		val = (CCPU(k_stack.sp) += degree);
	else
		val = (CCPU(u_stack.sp) += degree);
	return val;
}

cd_frame_t get_frame(pmode_t mode)
{
	return (mode == KERNAL) ? CCPU(k_cd) : CCPU(u_cd);
}

void set_frame(pmode_t mode, cd_frame_t frame)
{
	if (mode == KERNAL)
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
	if (memtype == ldst_fake)
	{
		return stream_in[address % (sizeof(stream_in) / sizeof(stream_in[0]))];
	}
	else
	{
		return memory_read(components.mem, address);
	}
}

void store(uint64_t address, int64_t value)
{
	if (memtype == ldst_fake)
	{
		stream_out[address % (sizeof(stream_out) / sizeof(stream_out[0]))] = value;
	}
	else
	{
		memory_write(components.mem, address, value);
	}
}

void step_cpu(void)
{
	static int cycles = 0;

	// printf("stage: %d\n", CCPU(stage));

	switch (CCPU(stage))
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

	if (CCPU(stage) == WRITEBACK)
	{
		// printf("sp: %d\n", get_sp());
		// printf("sfp: %d\n", get_sfp());
		CCPU(stage) = FETCH;
	}
	else
	{
		CCPU(stage) += 1;
	}
	// DROPCPU;
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
	CCPU(co) = 0;
	CCPU(ci1) = 0;
	CCPU(ci2) = 0;
	CCPU(ci3) = 0;

	uint64_t pc = get_pc() + get_pc_offset();
	uint64_t ipc = get_ipc() + get_ipc_offset();
#if DEBUG_MODE == 1
	// printf("fpc: %llu fipc: %llu\n", get_pc(), get_ipc());
	// printf("current imm: %d\n", CCPU(curimm));
	// printf("pc: %llu ipc: %llu\n", pc, ipc);

#endif
	CCPU(curins) = (int32_t)MEMLD(pc);
	CCPU(curimm) = (int64_t)MEMLD(ipc);

	CCPU(has_jumped) = false;
}
void decode_cpu(void)
{
	inst_t inst = decode_inst(CCPU(curins));
	print_inst(&inst);

#if DEBUG_MODE == 1

#endif
}
void execute_cpu(void)
{
	inst_t inst = decode_inst(CCPU(curins));

	if (inst.path == PATH_ALU)
	{

		alu_submit(components.alu, inst.subpath, get_reg(inst.rs1), get_reg(inst.rs2), CCPU(curimm), inst.immflag, inst.aux);

		alu_step(components.alu);

		CCPU(co) = components.alu->regdest;
	}
	if (inst.path == PATH_SYS)
	{
		system_submit(components.cpu);
	}
	if (inst.path == PATH_JMP)
	{

		jump_submit(components.cpu, inst.subpath, 0, get_reg(inst.rs1), get_reg(inst.rs2), CCPU(curimm), inst.immflag);
	}
	else if (inst.path == PATH_MEM)
	{
		memory_submit(components.cpu);
	}
}
void memory_cpu(void)
{
}
void writeback_cpu(void)
{

	inst_t inst = decode_inst(CCPU(curins));

	set_reg(inst.rd, CCPU(co));
	if (!CCPU(has_jumped))
	{
		inc_pc(1);
		inc_ipc((inst.immflag) ? 1 : 0);
	}
}

int64_t get_reg(int reg)
{
	//printf("get reg %d\n", reg);
	if (reg >= 64 || reg <= 0)
	{
		return 0;
	}
	if (CCPU(mode) == KERNAL)
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
	//printf("set reg %d %d\n", reg, content);
	if (reg >= 64 || reg <= 0)
	{
		return;
	}
	if (CCPU(mode) == KERNAL)
	{

		CCPU(kregs)
		[reg] = content;
	}
	else
	{
		CCPU(regs)
		[reg] = content;
	}
}

inst_t decode_inst(int32_t instr)
{
	inst_t in;
	in.path = (instr >> 28) & 0xF;
	in.subpath = (instr >> 21) & 0x7F;
	in.rd = (instr >> 15) & 0x3F;
	in.rs1 = (instr >> 9) & 0x3F;
	in.rs2 = (instr >> 3) & 0x3F;
	in.aux = (instr >> 1) & 0x03;
	in.immflag = instr & 0x1;
	return in;
}

int32_t encode_inst(inst_t *inst)
{

	return ((inst->path << 28) | (inst->subpath << 21) | (inst->rd << 15) | (inst->rs1 << 9) | (inst->rs2 << 3) | (inst->aux << 1) | inst->immflag);
}

uint64_t encode(uint64_t path, uint64_t subpath, uint64_t rd, uint64_t rs1, uint64_t rs2, uint64_t aux, uint64_t immf)
{
	uint64_t inst = ((path & 0xF) << 28) | ((subpath & 0x7F) << 21) | ((rd & 0x3f) << 15) | ((rs1 & 0x3f) << 9) | ((rs2 & 0x3f) << 3) | ((aux & 0xf) << 1) | (immf);
	// print_bin(inst, 32, 1);
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

	memory_t *mem = create_memory(10000);
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
	for (int i = 0; i < 64; ++i)
	{
		printf("reg:%d = %lld\n", i, get_reg(i));
	}
}
