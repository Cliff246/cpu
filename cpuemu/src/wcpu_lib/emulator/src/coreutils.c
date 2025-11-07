

#include "coreutils.h"




void push_scd(cpu_t *cpu, cd_frame_t frame)
{
	uint64_t stack_cd = (cpu->mode == KERNAL)? cpu->k_stack.scd: cpu->u_stack.scd;
	stack_cd -= 9;

	store(stack_cd, frame.cd_ptr);
	store(stack_cd + 1, frame.ct_ptr);
	store(stack_cd + 2, frame.ct_len);
	store(stack_cd + 3, frame.pc_ptr);
	store(stack_cd + 4, frame.pc_len);
	store(stack_cd + 5, frame.ipc_ptr);
	store(stack_cd + 6, frame.ipc_len);
	store(stack_cd + 7, frame.pc);
	store(stack_cd + 8, frame.ipc);


	if(cpu->mode == KERNAL)
		cpu->k_stack.scd = stack_cd;
	else
		cpu->u_stack.scd = stack_cd;
}

cd_frame_t pop_scd(cpu_t *cpu)
{
	uint64_t stack_cd = (cpu->mode == KERNAL)? cpu->k_stack.scd: cpu->u_stack.scd;



	cd_frame_t frame = {
		.cd_ptr = load(stack_cd),
		.ct_ptr = load(stack_cd + 1),
		.ct_len = load(stack_cd + 2),
		.pc_ptr = load(stack_cd + 3),
		.pc_len = load(stack_cd + 4),
		.ipc_ptr = load(stack_cd + 5),
		.ipc_len = load(stack_cd +6),
		.pc = load(stack_cd + 7),
		.ipc = load(stack_cd + 8)

	};

	if(cpu->mode == KERNAL)
		cpu->k_stack.scd = stack_cd + 9;
	else
		cpu->u_stack.scd = stack_cd + 9;
	return frame;
}
cd_frame_t get_frame_from_address(cpu_t *cpu, uint64_t address)
{
	int64_t cdtb_ptr = load(address);
	int64_t cdtb_len = load(address + 1);
	int64_t inst_ptr = load(address + 2);
	int64_t inst_len = load(address + 3);
	int64_t imm_ptr = load(address + 4);
	int64_t imm_len = load(address + 5);
	//set_pc(inst_ptr);
	//set_ipc(imm_ptr);
	cd_frame_t frame = {
		.cd_ptr = address,
		.ct_ptr = cdtb_ptr,
		.ct_len = cdtb_len,
		.ipc_ptr = imm_ptr,
		.ipc_len = imm_len,
		.pc_ptr = inst_ptr,
		.pc_len = inst_len,
		.ipc = 0,
		.pc = 0
	};
	return frame;
}