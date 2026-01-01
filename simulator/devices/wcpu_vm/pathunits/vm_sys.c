//

#include "vm_sys.h"
#include "vm.h"
#include "flags.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>





vm_op_status_t vm_SYS_SET_CD_PTR_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn)
{
	vm->control.load_cd = true;
	vm->control.wait_until_inst_end = true;
	vm->control.cd_base = txn->inp.lane1 + txn->inp.lane2 + txn->inp.lane3;
	vm->control.cd_count = false;

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
