//

#include "vm_sys.h"
#include "vm.h"
#include "flags.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>





vm_op_status_t vm_SYS_SET_CD_PTR_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{
	printf("set cd\n");
	vm->load_cd = true;
	vm->wait_until_inst_end = true;
	vm->cd_base = txn->inp.lane1 + txn->inp.lane2 + txn->inp.lane3;
	vm->cd_count = false;

	return VM_OP_STATUS_DONE;

}


vm_op_status_t vm_SYS_CALL_CD_PTR_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{

	return VM_OP_STATUS_DONE;
}

vm_op_status_t vm_SYS_BREAK_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{
	vm_cpu_print_regs(vm);
	return VM_OP_STATUS_DONE;

}
