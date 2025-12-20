#include "vm_eval.h"
#include "vm.h"
#include "vm_bus.h"
#include "vm_utils.h"
#include "isa.h"
#include "flags.h"
#include "vm_path.h"
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>



static void advance_eval_live_state(vm_eval_live_t *live)
{
	vm_eval_state_t to_set = VM_EVAL_STATE_DONE;
	switch(live->state)
	{
		case VM_EVAL_STATE_INIT:
			to_set = VM_EVAL_STATE_LOAD;
			break;
		case VM_EVAL_STATE_LOAD:
			to_set = VM_EVAL_STATE_EXEC;
			break;
		case VM_EVAL_STATE_EXEC:
			to_set = VM_EVAL_STATE_DONE;
			break;
		case VM_EVAL_STATE_DONE:
			break;

	}
	live->state = to_set;
}


static void reset_eval_live_state(vm_eval_live_t *live)
{
	live->waiting = false;
	memset(live->buffer, 0, sizeof(vm_transfer_t) * VM_EVAL_STATE_INIT);
	live->state = VM_EVAL_STATE_INIT;
}


void vm_eval_fetch_load(vima_t *vm)
{

	vm_eval_live_t *live = vm->live;

	
	switch(live->fetch.cycle)
	{
		case VM_EVAL_FETCH_INSTR:
			
			if(live->waiting == false)
			{

				uint64_t pc = vm_get_pc(vm);

				live->fetch.instr_addr = vm_get_pc_base(vm) + pc;
  

				vm_bus_add_transfer(vm, live->fetch.instr_addr);
				live->fetch.instr_even_or_odd = (pc % 2 == 0);
				live->waiting = true;
			}
			else
			{
				vm_transfer_t transfer =  vm_bus_has_data(vm, live->fetch.instr_addr);
				if(transfer.valid)
				{
					if(live->fetch.instr_even_or_odd)
					{
						 live->fetch.instr= (uint32_t)((uint64_t)(transfer.data >> 32) & 0xffffffff);
					}
					else
					{
						live->fetch.instr = (uint32_t)((transfer.data & 0xffffffff));
					}

					live->fetch.cycle = VM_EVAL_FETCH_IMM;
					live->waiting = false;
				}
				
			}


			break;

		case VM_EVAL_FETCH_IMM:
		
			if(live->waiting == false)
			{
				live->fetch.imm_addr = vm_get_ipc_base(vm) + vm_get_ipc(vm);
  

				vm_bus_add_transfer(vm, live->fetch.imm_addr);
				live->waiting = true;
			}
			else
			{
				vm_transfer_t transfer =  vm_bus_has_data(vm, live->fetch.imm_addr);
				if(transfer.valid)
				{
					live->fetch.imm = transfer.data;
					live->waiting = false;
					advance_eval_live_state(live);
				}
				
			}
		default:
			break;
	}

}

void vm_eval_fetch(vima_t *vm)
{

	vm_eval_live_t *live = vm->live;

	switch(live->state)
	{
		case VM_EVAL_STATE_INIT:
			live->fetch.cycle = 0;
			live->fetch.imm = 0;
			live->fetch.instr = 0;
			
			advance_eval_live_state(live);			
			break;
		case VM_EVAL_STATE_LOAD:
			vm_eval_fetch_load(vm);
			break;	
		case VM_EVAL_STATE_EXEC:
			advance_eval_live_state(live);
			break;

		case VM_EVAL_STATE_DONE:
			reset_eval_live_state(live);
			live->stage = VM_EVAL_STAGE_AGGREGATE;
			break;
		default:
			break;
	}
}




void vm_eval_aggregate(vima_t *vm)
{

	vm_eval_live_t *live = vm->live;
	switch(live->state)
	{

		case VM_EVAL_STATE_INIT:
			live->aggregate.inst = decode_inst(live->fetch.instr); 
			advance_eval_live_state(live);
			break;
		case VM_EVAL_STATE_LOAD:

			if(live->aggregate.inst.reallocflag)
			{
				if(!live->waiting)
				{
					live->waiting = true;
					vm_bus_add_transfer(vm, live->fetch.imm_addr);
				}	
				else
				{
					vm_transfer_t transfer =  vm_bus_has_data(vm, live->fetch.imm);
					if(transfer.valid)
					{
						live->aggregate.realloc = transfer.data;	
						live->waiting = false;
						advance_eval_live_state(live);
					}	
				}		
			}

			break;
		case VM_EVAL_STATE_EXEC:

			int64_t dest = 0;
			int64_t l1, l2, l3;
			int64_t sidestream = 0;
			inst_t inst  =live->aggregate.inst;
			bool swaps = inst.immflag;
			if(inst.immflag && inst.reallocflag)
			{
				sidestream = live->aggregate.realloc;
			}
			else if(inst.immflag)
			{
				sidestream = live->fetch.imm;
			}
			else if(inst.reallocflag)
			{
				sidestream = vm_cpu_get_reg(vm, 63);
			}
			else
			{
				sidestream = 0;
			}



			int64_t rs3_val = (inst.selflag)? inst.rs3 : vm_cpu_get_reg(vm, inst.rs3);

			if(inst.accflag)
			{
				dest = 63;
				l1 = vm_cpu_get_reg(vm, inst.rs1);
				l2 = vm_cpu_get_reg(vm, inst.rs2);
				l3 = rs3_val + sidestream;

			}
			else
			{
				dest = inst.rs1;
				l1 = vm_cpu_get_reg(vm, inst.rs2);
				l2 = rs3_val;
				l3 = sidestream;
			}
			live->aggregate.lane1 = l1;
			live->aggregate.lane2 = l1;
			live->aggregate.lane3 = l1;
			live->aggregate.dest = dest;
			advance_eval_live_state(live);
			break;
		case VM_EVAL_STATE_DONE:
			reset_eval_live_state(live);
			live->stage = VM_EVAL_STAGE_EXECUTE;
			break;
	}
}


void vm_eval_execute_load(vima_t *vm)
{

	vm_eval_live_t *live = vm->live;
	switch(live->exec.cycle)
	{
		case VM_EVAL_EXECUTE_JUMP:
			break;
		case VM_EVAL_EXECUTE_MEMORY:
			break;
		case VM_EVAL_EXECUTE_CD_SWAP:
			
			break;

	}
}



int64_t vm_eval_execute_get_opcode_load_length(vm_eval_live_t *live)
{
	inst_t inst = live->aggregate.inst;

	if(inst.path == PATH_JMP)
	{
		live->exec.cycle = VM_EVAL_EXECUTE_JUMP;
		return CODE_DESC_STRIDE + (CODE_DESC_STRIDE / 2) + 1;
	}
	if(inst.path == PATH_MEM)
	{
		live->exec.cycle = VM_EVAL_EXECUTE_MEMORY;
		return 1;
	}
	if(inst.path == PATH_SYS)
	{

		live->exec.cycle = VM_EVAL_EXECUTE_CD_SWAP;
		return 6;
	}
	live->exec.cycle = VM_EVAL_EXECUTE_NONE;
	return 0;	
}




void vm_eval_execute(vima_t *vm)
{

	vm_eval_live_t *live = vm->live;

	switch(live->state)
	{

		case VM_EVAL_STATE_INIT:
			live->exec.limit = vm_eval_execute_get_opcode_load_length(live);
			live->exec.current = 0;
			advance_eval_live_state(live);
			break;


		case VM_EVAL_STATE_LOAD:

			if(live->exec.cycle == VM_EVAL_EXECUTE_NONE)
			{
			
			}
			else
			{

			}

			break;
		case VM_EVAL_STATE_EXEC:

			inst_t inst = live->aggregate.inst; 
  			int64_t sets = vm_cpu_path_fn[inst.path].exec(vm, inst.subpath, live->aggregate.lane1, live->aggregate.lane2, live->aggregate.lane3, inst.immflag);
			live->exec.result = sets;
			advance_eval_live_state(live);
			break;
		case VM_EVAL_STATE_DONE:
			reset_eval_live_state(live);
			live->stage = VM_EVAL_STAGE_FINISH;
			break;
	}
}

void vm_eval_finish(vima_t *vm)
{


	vm_eval_live_t *live = vm->live;
}

void vm_eval(vima_t *vm)
{

	switch(vm->live->stage)
	{
		case VM_EVAL_STAGE_FETCH:

			vm_eval_fetch(vm);
			break;

		case VM_EVAL_STAGE_AGGREGATE:

			vm_eval_aggregate(vm);
			break;

		case VM_EVAL_STAGE_EXECUTE:

			vm_eval_execute(vm);
			break;

		case VM_EVAL_STAGE_FINISH:
			vm_eval_finish(vm);
			break;

		default:
			break;
		
	}
}


