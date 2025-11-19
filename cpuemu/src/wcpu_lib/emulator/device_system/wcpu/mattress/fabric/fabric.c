#include "fabric.h"

/*

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
	uint64_t pc = get_pc();
	uint64_t ipc = get_ipc();
	//printf("FETCH pc=%lu ipc=%lu mem[pc]=0x%08x\n", get_pc(), get_ipc(), get_inst_at_pc_address(get_pc()));
	//printf("FETCH: sp=%d sfp=%d\n", get_sp(), get_sfp());
	if(pc  > (get_pc_len() * 2))
	{
		CCPU(program_over) = true;
	}
	if(ipc > get_ipc_len())
	{
		CCPU(program_over) = true;
	}
#if DEBUG_MODE == 1
	// printf("fpc: %llu fipc: %llu\n", get_pc(), get_ipc());
	// printf("current imm: %d\n", CCPU(curimm));
	// printf("pc: %llu ipc: %llu\n", pc, ipc);

	//printf("pc %d\n", pc);
#endif


	uint32_t instbin  = (uint32_t)get_inst_at_pc_address(pc);
	int64_t immbin = (int64_t)MEMLD(ipc + get_ipc_offset());
	//printf("%d %lld\n", instbin, immbin);
	CCPU(op) = fill_operation(instbin, immbin);

	CCPU(has_jumped) = false;
}
void decode_cpu(void)
{
	decode_operation(&CCPU(op));
	//print_inst(&CCPU(op).inst.inst);
#if DEBUG_MODE == 1

#endif
}
void execute_cpu(void)
{
	inst_t inst = get_inst_from_op(&CCPU(op));


	int64_t rs1_n = inst.rs1, rs2_n = inst.rs2, rs3_n = inst.rs3;
	uint64_t imm = (inst.immflag)? CCPU(op).imm.imm : 0;

	if(inst.immflag == 0 && inst.reallocflag == 1)
	{

		imm = get_reg(REG_ACC);
	}

	//print_inst(&inst);
	int64_t rs1_d = (inst.accflag)? get_reg(rs1_n) : 0;
	int64_t rs2_d = get_reg(rs2_n);
	int64_t rs3_d = (inst.selflag)? rs3_n : get_reg(rs3_n);

	int64_t lane1;
	int64_t lane2;
	int64_t lane3;

	if(inst.accflag)
	{
		lane1 = rs1_d;
		lane2 = rs2_d;
		lane3 = rs3_d + imm;

	}
	else
	{
		lane1 = rs2_d;
		lane2 = rs3_d;
		lane3 = imm;
	}

	LOG("[accflag %d] [%d]=%lld [%d]=%lld [%d]=%lld\n",inst.accflag, rs1_n, lane1, rs2_n, lane2, rs3_n, lane3);


	if (inst.path == PATH_ALU)
	{



		//printf("%lld %lld \n", rs1_d, rs2_d);


		alu_submit(components.alu, inst.subpath, lane1, lane2, lane3, inst.immflag);
		alu_step(components.alu);
		CCPU(co) = components.alu->regdest;
		//printf("dest %lld\n",  components.alu->regdest);


	}
	else if (inst.path == PATH_SYS)
	{
		system_submit(components.cpu);
	}
	else if (inst.path == PATH_JMP)
	{

		jump_submit(components.cpu, inst.subpath, lane1, lane2, lane3, inst.immflag);
	}
	else if (inst.path == PATH_MEM)
	{
		memory_submit(components.cpu, inst.subpath, lane1, lane2, lane3, inst.immflag);
	}
}



void memory_cpu(void)
{
}
void writeback_cpu(void)
{

	inst_t inst = CCPU(op).inst.inst;


	if(inst.accflag)
	{
		//printf("%d\n", REG_ACC);

		set_reg(REG_ACC, CCPU(co));
	}
	else
	{
		//printf("%d\n", inst.rs1);
		set_reg(inst.rs1, CCPU(co));
	}

	if (!CCPU(has_jumped))
	{
		inc_pc(1);
		inc_ipc((inst.immflag) ? 1 : 0);
	}
}

*/