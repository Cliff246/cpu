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

//uint64_t find_immedate_from_rel_table(vima_t *cpu, uint64_t address);
//void jump_to(vima_t *vm, uint64_t address);
//void jump_submit(vima_t *cpu, uint64_t subpath, int64_t lane1, int64_t lane2, int64_t lane3, char immf);

void vm_cpu_path_jmp_init(vima_t *vm);

int64_t vm_cpu_path_jmp_exec(vima_t *vm, char subflag, int64_t lane1, int64_t lane2, int64_t lane3, bool swap);


#endif
