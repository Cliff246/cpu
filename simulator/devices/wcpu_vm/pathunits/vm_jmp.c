

#include "flags.h"
#include "vm.h"
#include "vm_path.h"
#include "vm_jmp.h"
#include "vm_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>




uint64_t bookmark_index(uint64_t address, uint64_t cd_ptr)
{
	uint64_t idx = (address ^ cd_ptr) & (BOOKMARKS_MAX - 1);
	return idx;
}

uint64_t get_from_bookmark(vima_t *vm, uint64_t rel_address, uint64_t cd_ptr)
{
    // hash on both rel_address and cd_ptr
    uint64_t idx = bookmark_index(rel_address, cd_ptr);

    if (vm->bookmarks[idx].valid &&
        vm->bookmarks[idx].inst_addr == rel_address &&
        vm->bookmarks[idx].cd_addr   == cd_ptr)
    {
        return vm->bookmarks[idx].imm_addr; // cache hit
    }
    else
    {
        return 0; // miss
    }
}


uint64_t closest_rel(vima_t *vm, uint64_t ct_addr, uint64_t ct_len, uint64_t rel_address)
{

	int64_t closest = 0;
	if(rel_address  != 0)
	{
		closest = rel_address / CODE_DESC_STRIDE;
	}

	uint64_t relative = vm_load(vm, ct_addr + closest);
	//printf("c:%ld r:%ld\n", closest, relative);
	return relative;
}

/*

uint64_t find_immediate_from_rel_table(vima_t *vm, uint64_t address)
{

	uint64_t closest = 0;
	///



		//does binary search on the table
	closest =  closest_rel(vm, ct_address, ct_len, address);
	uint64_t inst_addr = (address / CODE_DESC_STRIDE);
	uint64_t imm_addr = vm_load(vm, closest + ct_address);
	//printf("%d address\n", address);
	for(int i = 0; i < CODE_DESC_STRIDE; ++i)
	{
		uint32_t instruction = vm_get_inst_at_pc_address(vm, inst_addr);

		//printf("iaddr: %ld imm: %x addr: %d\n", inst_addr, instruction, address);
		if(address == inst_addr)
			break;
		if ((instruction & 0x1) > 0)
		{
			imm_addr++;
		}
		inst_addr++;

	}
	//printf("\ninst address: %llu with imm address:%llu\n\n", inst_addr, imm_addr );
	vm->bookmarks[idx].inst_addr = inst_addr ;
	vm->bookmarks[idx].imm_addr  = imm_addr;
	vm->bookmarks[idx].cd_addr = cd_ptr;
	vm->bookmarks[idx].valid = 1;
	//ADD TO BOOK MARKS
	return imm_addr;
}

*/

#define JMP_STATE_START 0
#define JMP_STATE_LOAD_CT_CLOSEST 1
#define JMP_STATE_LOAD_INST 2
#define JMP_STATE_DONE 3

#define JMP_STATE_LOAD_START 0
#define JMP_STATE_LOAD_WAIT 1
#define JMP_STATE_LOAD_DONE 2




vm_op_status_t vm_jump_address(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{

	//tua = address
	//tub = jmp state major
	//tuc = imm destination

	//tud = jmp state minor load
	//tue

	//tia = hnd
	uint64_t cd_ptr =  vm->regs.code_desc.cd_ptr;
	uint64_t ct_base = vm->regs.code_desc.ct_base;
	uint64_t ct_len = vm->regs.code_desc.ct_len;

	//check in book marks



	if(txn->local.tub == JMP_STATE_START)
	{
		uint64_t idx = bookmark_index(txn->local.tua, cd_ptr);

		if(vm->bookmarks[idx].valid && vm->bookmarks[idx].cd_addr == cd_ptr)
		{
			//printf("bookmarked\n");
			txn->local.tuc = get_from_bookmark(vm, txn->local.tua, cd_ptr);
			txn->local.tub = JMP_STATE_DONE;
			return VM_OP_STATUS_WAITING;


		}
		else
		{
			txn->local.tub = JMP_STATE_LOAD_CT_CLOSEST;
			txn->local.tud = JMP_STATE_LOAD_START;
			return VM_OP_STATUS_WAITING;

		}

	}
	else if(txn->local.tub == JMP_STATE_LOAD_CT_CLOSEST)
	{

		if(txn->local.tud == JMP_STATE_LOAD_START)
		{

			uint64_t closest = txn->local.tua / CODE_DESC_STRIDE;
			uint64_t address = closest + ct_base;

			vm_bus_evnt_t evnt =
			{
				.evnt.load = {.addr = address },
				.type = VM_IO_LOAD,
			};


			txn->local.tia = vm_bus_put_evnt(vm, txn->handle.port, evnt);
			txn->local.tud = JMP_STATE_LOAD_WAIT;
			return VM_OP_STATUS_LOADING;

		}
		else if(txn->local.tud == JMP_STATE_LOAD_WAIT)
		{
			if(vm_bus_poll_evnt(vm, txn->handle.port, txn->local.tia))
			{
				vm_bus_response_t response;
				vm_bus_pull_evnt(vm,txn->handle.port, txn->local.tia, &response);
				txn->local.tue = response.resp.load_response.value;
				txn->local.tud = JMP_STATE_LOAD_DONE;
				return VM_OP_STATUS_WAITING;

			}
			else
			{
				return VM_OP_STATUS_LOADING;
			}
		}
		else if(txn->local.tud == JMP_STATE_LOAD_DONE)
		{
			txn->local.tud = JMP_STATE_LOAD_START;
			txn->local.tub = JMP_STATE_LOAD_INST;
			return VM_OP_STATUS_WAITING;
		}
		else
		{
			assert(0);
		}






	}
	else if(txn->local.tub == JMP_STATE_LOAD_INST)
	{
		if(txn->local.tud == JMP_STATE_LOAD_START)
		{

			uint64_t address = (txn->local.tua / CODE_DESC_STRIDE) + vm_get_pc_base(vm) + txn->local.iter.x;
			vm_bus_evnt_t evnt =
			{
				.evnt.load = {.addr = address },
				.type = VM_IO_LOAD,
			};


			txn->local.tia = vm_bus_put_evnt(vm, txn->handle.port, evnt);
			txn->local.tud = JMP_STATE_LOAD_WAIT;

			return VM_OP_STATUS_LOADING;

		}
		else if(txn->local.tud == JMP_STATE_LOAD_WAIT)
		{
			if(vm_bus_poll_evnt(vm, txn->handle.port, txn->local.tia))
			{
				vm_bus_response_t response;
				vm_bus_pull_evnt(vm,txn->handle.port, txn->local.tia, &response);
				txn->local.tue = response.resp.load_response.value;
				txn->local.tud = JMP_STATE_LOAD_DONE;
				return VM_OP_STATUS_WAITING;

			}
			else
			{
				return VM_OP_STATUS_LOADING;
			}
		}
		else if(txn->local.tud == JMP_STATE_LOAD_DONE)
		{
			uint32_t inst0 = vm_get_inst_aligned_pc(txn->local.tue, false);


			if ((inst0 & 0x1) > 0)
			{
				txn->local.tuc++;
			}
			uint32_t inst1 =vm_get_inst_aligned_pc(txn->local.tue, true);


			if ((inst1 & 0x1) > 0)
			{
				txn->local.tuc++;

			}
			
			txn->local.iter.x++;

			txn->local.tub = JMP_STATE_LOAD_INST;
			return VM_OP_STATUS_WAITING;

		}
		else
		{
			assert(0);
		}

	}
	else if(txn->local.tub == JMP_STATE_DONE)
	{
		vm_set_pc(vm, txn->local.tua);
		vm_set_ipc(vm, txn->local.tuc);
		vm->txn->out.jumped = true;
		vm->txn->out.out = 0;
		return VM_OP_STATUS_DONE;

	}





	//uint64_t imm = find_immediate_from_rel_table(vm, address);

	//printf("\njumpto pc=%d ipc=%d\n\n", address, imm );
	//jump to

}





/*
void jump_call(vima_t *vm, uint64_t target, char immf)
{
	assert(0);
	/*
	store(inc_sp(1), get_pc() + 1);
	store(inc_sp(1), get_ipc() + ((immf)? 1: 0)) ;
	store(inc_sp(1), get_sfp());
	//memory_print(components.mem, 1000, 1010);
	//printf("\n\nipc %d\n\n", get_ipc());

	//printf("sp = %d\n", get_sp());
	set_sfp(get_sp());
	//printf("CALL before sp=%lu sfp=%lu\n", get_sp(), get_sfp());

	jump_to(cpu, target);
*/
/*

void jump_return(vima_t *vm)
{
	assert(0);
	set_sp(get_sfp());

    uint64_t sfp = load(dec_sp(1));   // last pushed
    uint64_t ipc = load(dec_sp(1));   // second pushed
    uint64_t pc  = load(dec_sp(1));
	//printf("RET after sp=%lu sfp=%lu\n", get_sp(), get_sfp());

	set_sfp(sfp);
	set_ipc(ipc);
	set_pc(pc);
	cpu->has_jumped = true;
	//printf("RET pop pc=%lu ipc=%lu sfp=%lu\n", pc, ipc, sfp);
}

void vm_cpu_path_jmp_init(vima_t *vm)
{

}

int64_t vm_cpu_path_jmp_exec(vima_t *vm, char subflag, int64_t lane1, int64_t lane2, int64_t lane3, bool swap)
{
	//printf("trying jump %d %d %d \n", lane1, lane2, lane3);
	switch(subflag)
	{
		case JP_JMP:
			//TODO revalute how this will be done
			jump_to(vm, lane1 + lane2 + lane3);
			break;
		case JP_BEQ:
			if(lane1 == lane2)
			{
				jump_to(vm, lane3);
			}
			break;
		case JP_BNE:
			if(lane1 != lane2)
			{
			 	jump_to(vm, lane3);

			}
			break;
		case JP_BLT:
			if((int64_t)lane1 < (int64_t)lane2)
			{
				jump_to(vm, lane3);
				//printf("branch less equals %d %d\n", lane1, lane2);

			}
			break;
		case JP_BLE:
			if((int64_t)lane1 <= (int64_t)lane2)
			{
				jump_to(vm, lane3);
				//printf("branch less than equals %d %d\n", lane1, lane2);
			}
			break;

		case JP_CALL:
			{

				//TODO revaluate if lane1 + lane2 + lane3 should be done
				jump_call(vm, lane1 + lane2 + lane3, swap);
			}
			break;
		case JP_RET:
			{
				jump_return(vm);
			}
			break;
		case JP_BLEU:
			if((uint64_t)lane1 <= (uint64_t)lane2)
			{
				jump_to(vm, lane3);
			}
			break;

		case JP_BLTU:
			if((uint64_t)lane1 < (uint64_t)lane2)
			{
				jump_to(vm, lane3);
			}
			break;

		default:
			break;

	}
	return 0;
}
*/

vm_op_status_t vm_JMP_JMP_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{
	if(txn->local.tub == 0)
	{
		uint64_t sum = txn->inp.lane1 +  txn->inp.lane2 +  txn->inp.lane3;
		txn->local.tua = sum;

	}

	return vm_jump_address(vm, op, txn);

}

vm_op_status_t vm_JMP_BEQ_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{

	if(txn->inp.lane1 ==  txn->inp.lane2)
	{

		if(txn->local.tub == 0)
		{
			uint64_t sum = txn->inp.lane3;
			txn->local.tua = sum;

		}
		return vm_jump_address(vm, op, txn);
	}
	else
	{
		return VM_OP_STATUS_DONE;
	}






}

vm_op_status_t vm_JMP_BNE_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{
	return VM_OP_STATUS_DONE;

}

vm_op_status_t vm_JMP_BLE_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{
	return VM_OP_STATUS_DONE;

}

vm_op_status_t vm_JMP_BLT_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{
	return VM_OP_STATUS_DONE;

}

vm_op_status_t vm_JMP_CALL_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{
	return VM_OP_STATUS_DONE;

}

vm_op_status_t vm_JMP_RET_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{
	return VM_OP_STATUS_DONE;

}

vm_op_status_t vm_JMP_BLEU_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{
	return VM_OP_STATUS_DONE;

}

vm_op_status_t vm_JMP_BLTU_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{
	return VM_OP_STATUS_DONE;

}
