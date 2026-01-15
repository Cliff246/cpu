#include "vm_txn.h"
#include "commons.h"
#include "vm.h"
#include "vm_op.h"
#include "vm_regs.h"
#include "vm_utils.h"
#include "vm_bus.h"
#include "vm_path.h"
#include "flags.h"

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

void vm_txn_set_next_state(vm_txn_t *txn, vm_txn_state_t state)
{
	txn->next.next = state;
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

void vm_txn_reset_local(vm_txn_t *txn)
{
	txn->local.iter.lim = 0;
	txn->local.iter.x = 0;
	txn->local.iter.y = 0;
	txn->local.tia = 0;
	txn->local.tib = 0;
	txn->local.tic = 0;
	txn->local.tid = 0;
	txn->local.tua = 0;
	txn->local.tub = 0;
	txn->local.tuc = 0;
	txn->local.tud = 0;
	txn->local.tuf = 0;
	txn->local.tue = 0;
	txn->local.tug = 0;
	for(int i = 0; i < VM_TXN_MAX_WORKING_SPACE; ++i)
	{
		txn->local.working[i] = 0;
	}
}


void vm_txn_state_inval(vima_t *vm, vm_txn_t *txn)
{
	assert(0);

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

	vm_txn_set_next_state(txn, VM_TXN_FETCH);


}


#define _TXN_FETCH_T1_STATE_START 0
#define _TXN_FETCH_T1_STATE_INST 1
#define _TXN_FETCH_T1_STATE_IMM 2

void vm_txn_state_fetch(vima_t *vm, vm_txn_t *txn)
{

	if(txn->private.it1 > 0)
	{
		vm_bus_hnd_t hnd = (vm_bus_hnd_t)txn->private.it2;
		//use local s1
		if(txn->private.it1 == _TXN_FETCH_T1_STATE_INST)
		{
			if(vm_bus_poll_evnt(vm, txn->handle.port, hnd))
			{
				vm_bus_response_t resp;
				assert(vm_bus_pull_evnt(vm, txn->handle.port, hnd, &resp));

				bool even_odd = txn->op.pc % 2 == 0;
				//printf("\n\n%d\n\n", resp.resp.load_response.value);
				txn->op.inst = vm_get_inst_aligned_pc(resp.resp.load_response.value, even_odd);

				vm_bus_evnt_t evnt =
				{
					.evnt.load = {.addr = txn->op.ipc + txn->private.ut2},
					.type = VM_IO_LOAD,
				};

				txn->private.it2 = vm_bus_put_evnt(vm, txn->handle.port, evnt);
				assert(txn->private.it2 >= 0);
				txn->private.it1 = _TXN_FETCH_T1_STATE_IMM;
			}
		}
		else if(txn->private.it1 == _TXN_FETCH_T1_STATE_IMM)
		{
			if(vm_bus_poll_evnt(vm, txn->handle.port, hnd))
			{
				vm_bus_response_t resp;
				assert(vm_bus_pull_evnt(vm, txn->handle.port, hnd, &resp));

				txn->op.imm = resp.resp.load_response.value;
				vm_txn_set_next_state(txn, VM_TXN_DECODE);
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
		txn->private.ut1 = vm_get_pc_base(vm);
		txn->private.ut2 = vm_get_ipc_base(vm);
		//printf("\n\nbefore pc:%lld\n\n", txn->private.ut1);
		vm_bus_evnt_t evnt =
		{
			.evnt.load = {.addr = ((txn->op.pc / 2) + txn->private.ut1)},
			.type = VM_IO_LOAD,
		};
		txn->private.it1 = _TXN_FETCH_T1_STATE_INST;
		txn->private.it2 = vm_bus_put_evnt(vm, txn->handle.port, evnt);
		//vm_print_bus(vm);
		//vm_txn_print_local(txn);
	}


}
void vm_txn_state_decode(vima_t *vm, vm_txn_t *txn)
{


	txn->op.op = decode_inst(txn->op.inst);
	vm_txn_set_next_state(txn, VM_TXN_SOURCE);


}

#define _TXN_SOURCE_T2_STATE_START 0
#define _TXN_SOURCE_T2_STATE_POLL 1

void vm_txn_state_source(vima_t *vm, vm_txn_t *txn)
{

	int64_t sidestream = 0;

	if(txn->op.op.immflag)
	{
		sidestream = txn->op.imm;
	}

	txn->srcs.rs1 = vm_cpu_get_reg(vm, txn->op.op.rs1);
	txn->srcs.rs2 = vm_cpu_get_reg(vm, txn->op.op.rs2);
	txn->srcs.rs3 = (txn->op.op.selflag)? txn->op.op.rs3 : vm_cpu_get_reg(vm, txn->op.op.rs3);
	txn->srcs.side = sidestream;
	vm_txn_set_next_state(txn, VM_TXN_WIRE);

}


void vm_txn_state_wire(vima_t *vm, vm_txn_t *txn)
{
	int modeflag = txn->op.op.modeflag;
	printf("%d\n", modeflag);
	if(modeflag == MODE_DEFAULT)
	{
		txn->inp.dst = txn->op.op.rs1;
		txn->inp.lane1 = txn->srcs.rs2;
		txn->inp.lane2 = txn->srcs.rs3;
		txn->inp.lane3 = txn->srcs.side;


	}
	else if(modeflag == MODE_UNDECIDED)
	{
		assert(0);

	}
	else if(modeflag == MODE_ACCUMLATOR)
	{
		txn->inp.dst = 63;
		txn->inp.lane1 = txn->srcs.rs1;
		txn->inp.lane2 = txn->srcs.rs2;
		txn->inp.lane3 = txn->srcs.rs3 + txn->srcs.side;

	}
	else if(modeflag == MODE_SINK)
	{
		txn->inp.dst = 0;
		txn->inp.lane1 = txn->srcs.rs1;
		txn->inp.lane2 = txn->srcs.rs2;
		txn->inp.lane3 = txn->srcs.rs3 + txn->srcs.side;


	}
	else
	{
		assert(0);
	}
	txn->inp.swap = txn->op.op.immflag;
	vm_txn_set_next_state(txn, VM_TXN_EXEC);

}




void vm_txn_state_exec(vima_t *vm, vm_txn_t *txn)
{
	int path = txn->op.op.path;
	int subpath = txn->op.op.subpath;
	//printf("exec %d %d\n",path, subpath);

	vm_op_t *op =  &vm_wcpu_ops[path].ops[subpath];

	txn->inflight.op = op;
	txn->inflight.status = op->run(vm, op, txn);


	if(txn->inflight.status == VM_OP_STATUS_DONE)
	{
		vm_txn_set_next_state(txn, VM_TXN_OUTPUT);
	}



}

void vm_txn_state_output(vima_t *vm, vm_txn_t *txn)
{
	vm_cpu_set_reg(vm, txn->inp.dst, txn->out.out);
	//printf("register: %d %d\n",txn->inp.dst, txn->out.out);
	//vm_cpu_print_regs(vm);
	txn->next.next = VM_TXN_RETIRE;
	vm_txn_set_next_state(txn, VM_TXN_RETIRE);


}

void vm_txn_state_retire(vima_t *vm, vm_txn_t *txn)
{
	//printf("op:%s pc:%d ipc:%d\n", txn->inflight.op->string, txn->op.pc, txn->op.ipc);
	//printf("sp:%d sfp:%d\n", vm_get_sp(vm), vm_get_sfp(vm));
	if(!txn->out.jumped)
	{
		if(txn->op.op.immflag)
		{
			vm_inc_ipc(vm, 1);
		}
		vm_inc_pc(vm, 1);
	}


	txn->next.next = VM_TXN_DONE;
	vm_txn_set_next_state(txn, VM_TXN_DONE);

}

void vm_txn_state_done(vima_t *vm, vm_txn_t *txn)
{
	vm->control.new_inst = true;

	vm_bus_free_port(vm, txn->handle.port);
}



void vm_txn_switch_state(vima_t *vm, vm_txn_t *txn)
{
	if(txn->next.next == VM_TXN_INVAL)
	{
		txn->state = txn->state;
		txn->next.next = VM_TXN_INVAL;
		return;
	}
	else
	{

		txn->private.ut1 = 0;
		txn->private.ut2 = 0;
		txn->private.ut3 = 0;
		txn->private.ut4 = 0;


		txn->private.it1 = 0;
		txn->private.it2 = 0;
		txn->private.it3 = 0;
		txn->private.it4 = 0;

		vm_txn_reset_local(txn);



		txn->state = txn->next.next;
		txn->next.next = VM_TXN_INVAL;
	}
}

void vm_txn_advance(vima_t *vm, vm_txn_t *txn)
{
	if( txn->inflight.op != NULL)
	{
	}
	txn_state_handle_ary[txn->state](vm, txn);
	vm_txn_switch_state(vm, txn);

}

vm_txn_t *vm_txn_create(vima_t *vm)
{
	vm_txn_t *txn =  calloc(1, sizeof(vm_txn_t));
	txn->state = VM_TXN_INIT;
	return txn;
}

void vm_txn_print_op(vm_txn_t *txn)
{
	print_inst(&txn->op.op);
	printf("pc:%llu ipc:%llu inst:%d imm:%lld\n", txn->op.pc, txn->op.ipc, txn->op.inst, txn->op.imm);
}
void vm_txn_print_src(vm_txn_t *txn)
{
	printf("rs1:%lld rs2:%lld rs3: %lld side: %lld\n", txn->srcs.rs1,txn->srcs.rs1,txn->srcs.rs1, txn->srcs.side);
}
void vm_txn_print_inp(vm_txn_t *txn)
{
	printf("dst: %d lane1: %lld lane2: %lld lane3: %lld swap %d\n", txn->inp.dst, txn->inp.lane1,  txn->inp.lane2,  txn->inp.lane2,  txn->inp.swap);
}

void vm_txn_print_handle(vm_txn_t *txn)
{
	printf("handle: %d\n", txn->handle.port);
}
void vm_txn_print_inflight(vm_txn_t *txn)
{
	if(txn->inflight.op)
	{
		printf("op:%s \n", txn->inflight.op->string);
		printf("state:%s\n", vm_op_status_str[txn->inflight.status]);
	}
}

void vm_txn_print_private(vm_txn_t *txn)
{
	printf("it1:%lld it2:%lld it3:%lld it4:%lld\n", txn->private.it1, txn->private.it2,  txn->private.it3, txn->private.it4);
	printf("ut1:%llu ut2:%llu ut3:%llu ut4:%llu\n", txn->private.ut1, txn->private.ut2,  txn->private.ut3,  txn->private.ut4);
}
void vm_txn_print_next(vm_txn_t *txn)
{
	printf("next: %s\n", vm_txn_state_str[txn->next.next]);
}

void vm_txn_print_out(vm_txn_t *txn)
{
	printf("jumped: %lld value:%lld\n", txn->out.jumped, txn->out.out);
}

void vm_txn_print_status(vm_txn_t *txn)
{
	printf("state: %s\n", vm_txn_state_str[txn->state]);

}

void vm_txn_print_local(vm_txn_t *txn)
{
	printf("tua: %llu tub:%llu tuc: %llu tud: %llu tue: %llu tuf: %llu tug: %llu\n", txn->local.tua, txn->local.tub, txn->local.tuc, txn->local.tud, txn->local.tue, txn->local.tuf, txn->local.tug);
	printf("tia: %lld tib:%lld tic: %lld tid: %lld\n", txn->local.tia, txn->local.tib, txn->local.tic, txn->local.tid);
	printf("x: %llu y: %llu lim: %llu\n", txn->local.iter.x,  txn->local.iter.y, txn->local.iter.lim);
	for(int i = 0; i < VM_TXN_MAX_WORKING_SPACE; ++i)
	{
		printf("working[%d] = %lld\n", i, txn->local.working[i]);
	}
}

void vm_txn_print(vm_txn_t *txn)
{
	vm_txn_print_status(txn);
	vm_txn_print_inflight(txn);
	vm_txn_print_op(txn);
	vm_txn_print_src(txn);
	vm_txn_print_inp(txn);
	vm_txn_print_out(txn);
	vm_txn_print_next(txn);
	vm_txn_print_private(txn);
	printf("\n");


}
