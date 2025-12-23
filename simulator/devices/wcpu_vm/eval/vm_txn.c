#include "vm_txn.h"
#include "commons.h"
#include "vm.h"
#include "vm_op.h"
#include "vm_regs.h"
#include "vm_utils.h"
#include "vm_bus.h"
#include "vm_path.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define VM_TXN_STATE_ARRAY(X) [VM_TXN_STATE_NAME(X)] = #X,

char *vm_txn_state_str[VM_TXN_STATE_COUNT] =
{
	VM_TXN_STATE_LIST(VM_TXN_STATE_ARRAY)
};

#define VM_TXN_FLAG_ARRAY(Y, X) [VM_TXN_FLAG_NAME(X)] = #X,
char *vm_txn_flag_str[VM_TXN_FLAG_COUNT] =
{
	VM_TXN_FLAG_LIST(VM_TXN_FLAG_ARRAY)
};

#define VM_TXN_FLAG_MASK_ARRAY(Y, X) [VM_TXN_FLAG_NAME(X)] = VM_TXN_FLAG_MASK_NAME(X),

vm_txn_flag_mask_t vm_txn_flag_mask_ary[VM_TXN_FLAG_COUNT] =
{
	VM_TXN_FLAG_LIST(VM_TXN_FLAG_MASK_ARRAY)
};


bool vm_txn_check_flag(vm_txn_t *txn, vm_txn_flag_t flag)
{

	uint64_t mask = vm_txn_flag_mask_ary[flag];
	uint64_t chunk = mask / 8;
	uint64_t index = mask % 8;
 	bool check = GETBIT(txn->flags[chunk], index);
	return check;
}
void vm_txn_set_flag(vm_txn_t *txn, vm_txn_flag_t flag)
{
	uint64_t mask = vm_txn_flag_mask_ary[flag];
	uint64_t chunk = mask / 8;
	uint64_t index = mask % 8;
 	char set_flag  = SETBIT(txn->flags[chunk], index);
	txn->flags[chunk] = set_flag;
}


char *vm_txn_flag_to_str(vm_txn_flag_t flag)
{
	return vm_txn_flag_str[flag];
}

char *vm_txn_flag_mask_to_str(vm_txn_flag_mask_t flag)
{

	for(int i = 0; i < VM_TXN_FLAG_COUNT; ++i)
	{
		if(flag == vm_txn_flag_mask_ary[i])
		{
			return vm_txn_flag_str[i];
		}
	}
	return vm_txn_flag_str[0];
}



void vm_txn_state_inval(vima_t *vm, vm_txn_t *txn)
{

}
void vm_txn_state_fault(vima_t *vm, vm_txn_t *txn)
{
	assert(0);
}

void vm_txn_state_init(vima_t *vm, vm_txn_t *txn)
{
	//todo temp
	vm_bus_port_id_t pid = vm_bus_create_port(vm, VM_ADDR_KERNAL);
	txn->handle.port = pid;

	txn->next.held = false;
	txn->next.next = true;


}


#define _TXN_FETCH_T1_STATE_INST 0
#define _TXN_FETCH_T1_STATE_IMM 1

void vm_txn_state_fetch(vima_t *vm, vm_txn_t *txn)
{

	if(txn->next.held)
	{
		vm_bus_hnd_t hnd = (vm_bus_hnd_t)txn->local.t2;
		//use local s1
		if(txn->local.t1 == _TXN_FETCH_T1_STATE_INST)
		{
			if(vm_bus_poll_evnt(vm, txn->handle.port, hnd))
			{
				vm_bus_response_t resp;
				assert(vm_bus_pull_evnt(vm, txn->handle.port, hnd, &resp));

				bool even_odd = txn->op.pc % 2 == 0;

				txn->op.inst = vm_get_inst_aligned_pc(resp.resp.load_response.value, even_odd);

				vm_bus_evnt_t evnt =
				{
					.evnt.load = {.addr = txn->op.ipc},
					.type = VM_IO_LOAD,
				};

				txn->local.t2 = vm_bus_put_evnt(vm, txn->handle.port, evnt);
				assert(txn->local.t2 >= 0);
				txn->local.t1 = _TXN_FETCH_T1_STATE_IMM;

			}

			txn->next.held = true;
		}
		else if(txn->local.t1 == _TXN_FETCH_T1_STATE_IMM)
		{
			if(vm_bus_poll_evnt(vm, txn->handle.port, hnd))
			{
				vm_bus_response_t resp;
				assert(vm_bus_pull_evnt(vm, txn->handle.port, hnd, &resp));

				txn->op.imm = resp.resp.load_response.value;
				txn->next.held = false;
				txn->next.next = VM_TXN_DECODE;

			}
			else
			{
				txn->next.next = VM_TXN_FETCH;
				txn->next.held = true;
			}
		}
		else
		{
			assert(0);
		}


	}
	else
	{
		txn->op.pc = vm_get_pc(vm);
		txn->op.ipc = vm_get_ipc(vm);
		txn->local.t3 = vm_get_pc_base(vm);
		txn->local.t4 = vm_get_ipc_base(vm);
		vm_bus_evnt_t evnt =
		{
			.evnt.load = {.addr = txn->op.pc + txn->local.t3},
			.type = VM_IO_LOAD,
		};
		txn->local.t1 = _TXN_FETCH_T1_STATE_INST;
		txn->local.t2 = vm_bus_put_evnt(vm, txn->handle.port, evnt);

		txn->next.next = VM_TXN_FETCH;
		txn->next.held = true;
	}


}
void vm_txn_state_decode(vima_t *vm, vm_txn_t *txn)
{


	txn->op.op = decode_inst(txn->op.inst);
	txn->next.next = VM_TXN_SOURCE;
}

void vm_txn_state_source(vima_t *vm, vm_txn_t *txn)
{

	int64_t sidestream = 0;
	if(txn->next.held)
	{
		if(vm_bus_poll_evnt(vm, txn->handle.port, txn->local.t1))
		{

			vm_bus_response_t resp;
			assert(vm_bus_pull_evnt(vm, txn->handle.port, txn->local.t1, &resp));
			sidestream = resp.resp.load_response.value;

		}
		else
		{

			txn->next.held = true;
			return;
		}
	}
	else
	{
		if(txn->op.op.immflag && txn->op.op.reallocflag)
		{

		//sidestream = vm_load(vm, txn->op);
		//
		//
			vm_bus_evnt_t event = {
				.type = VM_IO_LOAD,
				.evnt.load = {.addr = (uint64_t)txn->op.imm}
			};
			vm_bus_hnd_t hnd = vm_bus_put_evnt(vm, txn->handle.port, event);
			txn->local.t1 = hnd;
			txn->next.held = true;
			return;
		}
		else if(txn->op.op.immflag)
		{
			sidestream = txn->op.imm;
		}
		else if(txn->op.op.reallocflag)
		{
			sidestream = vm_cpu_get_reg(vm, 63);
		}
		else
		{
			sidestream = 0;
		}
	}




	txn->srcs.rs1 = vm_cpu_get_reg(vm, txn->op.op.rs1);
	txn->srcs.rs2 = vm_cpu_get_reg(vm, txn->op.op.rs2);
	txn->srcs.rs3 = (txn->op.op.selflag)? txn->op.op.rs3 : vm_cpu_get_reg(vm, txn->op.op.rs3);
	txn->srcs.side = sidestream;
	txn->next.next = VM_TXN_WIRE;
	txn->next.held = false;

}


void vm_txn_state_wire(vima_t *vm, vm_txn_t *txn)
{
	if(txn->op.op.accflag)
	{
		txn->inp.dst = 63;
		txn->inp.lane1 = txn->srcs.rs1;
		txn->inp.lane2 = txn->srcs.rs2;
		txn->inp.lane3 = txn->srcs.rs3 + txn->srcs.side;

	}
	else
	{
		txn->inp.dst = txn->srcs.rs1;
		txn->inp.lane1 = txn->srcs.rs2;
		txn->inp.lane2 = txn->srcs.rs3;
		txn->inp.lane3 = txn->srcs.side;

	}
	txn->inp.swap = txn->op.op.immflag;
	txn->next.next = VM_TXN_EXEC;
	txn->next.held = false;

}


void vm_txn_state_hold(vima_t *vm, vm_txn_t *txn)
{

}

void vm_txn_state_exec(vima_t *vm, vm_txn_t *txn)
{

	int path = txn->op.op.path;
	int subpath = txn->op.op.subpath;
	vm_op_t *op =  &vm_wcpu_ops[path].ops[subpath];

	txn->inflight.op = op;
	txn->inflight.status = op->run(vm, op, txn);


	if(txn->inflight.status != VM_OP_STATUS_DONE)
	{
		txn->next.next = VM_TXN_EXEC;

		txn->next.held = true;

	}
	else
	{
		txn->next.next = VM_TXN_OUTPUT;
		txn->next.held = false;
	}


}

void vm_txn_state_output(vima_t *vm, vm_txn_t *txn)
{
	vm_cpu_set_reg(vm, txn->out.dst, txn->out.out);

	txn->next.next = VM_TXN_RETIRE;
		txn->next.held = false;

}

void vm_txn_state_retire(vima_t *vm, vm_txn_t *txn)
{
}

void vm_txn_state_done(vima_t *vm, vm_txn_t *txn)
{

}



void vm_txn_switch_state(vima_t *vm, vm_txn_t *txn)
{
	if(txn->next.held)
	{
		txn->state = txn->state;
		return;
	}

	txn->local.t1 = 0;
	txn->local.t2 = 0;
	txn->local.t3 = 0;
	txn->local.t4 = 0;

	txn->iter.lim = 0;
	txn->iter.x = 0;
	txn->iter.y = 0;



	txn->state = txn->next.next;
	txn->next.next = VM_TXN_INVAL;
}

void vm_txn_advance(vima_t *vm, vm_txn_t *txn)
{


	txn_state_handle_ary[txn->state](vm, txn);


}
