#ifndef __VM_CPU_PATH_SYS_HEADER__
#define __VM_CPU_PATH_SYS_HEADER__

#include "vm.h"
#include "vm_op.h"
#include "vm_txn.h"

vm_op_status_t vm_SYS_SET_CD_PTR_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_SYS_CALL_CD_PTR_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_SYS_BREAK_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);



#endif
