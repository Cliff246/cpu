#ifndef __VM_CPU_PATH_MEMORY_HEADER__
#define __VM_CPU_PATH_MEMORY_HEADER__
#include "vm.h"
#include "vm_op.h"
#include "vm_txn.h"
#include <stdint.h>
#include <stdbool.h>
#include "vm_path.h"

vm_op_status_t vm_MEM_LDI_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_MEM_STI_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_MEM_SP_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_MEM_SFP_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_MEM_LDS_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_MEM_STS_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_MEM_INCSP_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_MEM_DECSP_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_MEM_PUSH_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_MEM_POP_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);


#endif
