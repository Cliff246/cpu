

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

	//printf("[%d] = %d\n", txn->inp.lane2 + txn->inp.lane3, txn->inp.lane1);
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

	//printf("push: %d\n", vm_get_sp(vm));
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
		//printf("pop: %d\n", vm_get_sp(vm));

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



