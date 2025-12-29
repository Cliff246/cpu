#ifndef __VM_CPU_PATH_JMP_HEADER__
#define __VM_CPU_PATH_JMP_HEADER__


#include "vm.h"
#include "vm_op.h"
#include "vm_txn.h"
#include "vm.h"
#include <stdint.h>
#include <stdbool.h>


vm_op_status_t vm_JMP_JMP_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_JMP_BEQ_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_JMP_BNE_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_JMP_BLE_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_JMP_BLT_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_JMP_CALL_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_JMP_RET_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_JMP_BLEU_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_JMP_BLTU_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);


#endif
