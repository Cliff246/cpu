

#include "vm.h"
#include "vm_op.h"
#include "vm_txn.h"
#include "vm_mem.h"
#include "vm_utils.h"
#include "flags.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>


vm_op_status_t vm_MEM_LDI_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{

	if(txn->local.tia == 0)
	{
		uint64_t sum = txn->inp.lane1 + txn->inp.lane2 + txn->inp.lane3;

		vm_bus_evnt_t evnt =
		{
			.evnt.load = {.addr = sum, },
			.type = VM_IO_LOAD,
		};

		txn->local.tib = vm_bus_put_evnt(vm, txn->handle.port, evnt);
		txn->local.tia = 1;
		return VM_OP_STATUS_TRANSITION;

	}
	else if(txn->local.tia == 1)
	{
		if(vm_bus_poll_evnt(vm, txn->handle.port, txn->local.tib))
		{
			vm_bus_response_t response;
			vm_bus_pull_evnt(vm, txn->handle.port, txn->local.tib, &response);
			//printf("loads %lld\n", response.resp.load_response.value);
			txn->out.out = response.resp.load_response.value;
			return VM_OP_STATUS_DONE;

		}
		else
		{
			return VM_OP_STATUS_WAITING;

		}

	}


	return VM_OP_STATUS_INVAL;



}
vm_op_status_t vm_MEM_STI_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{


	uint64_t sum = txn->inp.lane2 + txn->inp.lane3;
	int64_t val = txn->inp.lane1;
	vm_bus_evnt_t evnt =
	{
		.evnt.store = {.addr = sum, .val = val },
		.type = VM_IO_STORE
	};

	txn->local.tib = vm_bus_put_evnt(vm, txn->handle.port, evnt);
	return VM_OP_STATUS_DONE;


}
vm_op_status_t vm_MEM_SP_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{


	txn->out.out = vm_get_sp(vm);

	if(txn->inp.swap)
	{
		uint64_t sum = txn->inp.lane1 +txn->inp.lane2 + txn->inp.lane3;

		vm_set_sp(vm, sum);
	}


	return VM_OP_STATUS_DONE;

}
vm_op_status_t vm_MEM_SFP_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{
	txn->out.out = vm_get_sfp(vm);

	if(txn->inp.swap)
	{
		uint64_t sum = txn->inp.lane1 +txn->inp.lane2 + txn->inp.lane3;

		vm_set_sfp(vm, sum);
	}
	return VM_OP_STATUS_DONE;

}
vm_op_status_t vm_MEM_LDS_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{
	return VM_OP_STATUS_DONE;

}
vm_op_status_t vm_MEM_STS_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{
	return VM_OP_STATUS_DONE;

}

vm_op_status_t vm_MEM_INCSP_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{
	return VM_OP_STATUS_DONE;

}

vm_op_status_t vm_MEM_DECSP_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{
	return VM_OP_STATUS_DONE;

}

vm_op_status_t vm_MEM_PUSH_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{

	printf("push: %d\n", vm_get_sp(vm));
	int64_t val = txn->inp.lane1;
	vm_bus_evnt_t evnt =
	{
		.evnt.store = {.addr = vm_inc_sp(vm, 1), .val = val },
		.type = VM_IO_STORE
	};

	txn->local.tib = vm_bus_put_evnt(vm, txn->handle.port, evnt);
	return VM_OP_STATUS_DONE;

}

vm_op_status_t vm_MEM_POP_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{


	if(txn->local.tia == 0)
	{
		printf("pop: %d\n", vm_get_sp(vm));

		vm_bus_evnt_t evnt =
		{
			.evnt.load = {.addr = vm_dec_sp(vm, 1)},
			.type = VM_IO_LOAD,
		};

		txn->local.tib = vm_bus_put_evnt(vm, txn->handle.port, evnt);
		txn->local.tia = 1;
		return VM_OP_STATUS_TRANSITION;

	}
	else if(txn->local.tia == 1)
	{
		if(vm_bus_poll_evnt(vm, txn->handle.port, txn->local.tib))
		{
			vm_bus_response_t response;
			vm_bus_pull_evnt(vm, txn->handle.port, txn->local.tib, &response);
			//printf("loads %lld\n", response.resp.load_response.value);
			txn->out.out = response.resp.load_response.value;
			return VM_OP_STATUS_DONE;

		}
		else
		{
			return VM_OP_STATUS_WAITING;

		}

	}


	return VM_OP_STATUS_INVAL;




}


void vm_cpu_path_mem_init(vima_t *vm)
{

}
/*
int64_t vm_cpu_path_mem_exec(vima_t *vm, char subflag, int64_t lane1, int64_t lane2, int64_t lane3, bool swap)
{
	uint64_t address, sd, ld;
	int64_t result = 0;
	switch(subflag)
	{
		case MEM_LDI:
			address = lane1 + lane2 + lane3;
			ld = vm_load(vm, address);
			//printf("load [[%d+%d+%d]=%d] = %d\n", lane1, lane2, lane3, address, ld);
			result = ld;
			break;
		case MEM_STI:
			address = lane2 + lane3;
			//printf("store [[%d+%d]=%d] = %d \n",lane2, lane3, address, lane1 );

			vm_store(vm, address, lane1);
			break;

		case MEM_SP:
			//dest to co
			result = vm_get_sp(vm);
			vm_set_sp(vm,lane1 + lane2 + lane3);

			break;
		case MEM_SFP:
			//dest to co
			result = vm_get_sfp(vm);
			vm_set_sfp(vm,lane1 + lane2 + lane3);

			break;
		case MEM_LDS:
			address = vm_get_sp(vm) + lane2 + lane3;
			ld = vm_load(vm, address);
			//printf("load %lld = %lld\n", address, ld);
			result = ld;
			break;

		case MEM_STS:
			address = vm_get_sp(vm) + lane2 + lane3;
			//printf("store [%lld] = %lld \n", address, lane1);

			vm_store(vm, address, lane1);
			break;
		case MEM_INCSP:

			vm_inc_sp(vm,lane2 + lane3);
			break;
		case MEM_DECSP:
			vm_dec_sp(vm,lane2 + lane3);

			break;
		case MEM_PUSH:

			address = vm_inc_sp(vm,1);

			vm_store(vm, address, lane1);

			break;

		case MEM_POP:
			address = vm_dec_sp(vm,1);
			result = vm_load(vm, address);
			break;
		default:
			//printf("nothing done\n");
			break;
	}
	return result;
}

*/
