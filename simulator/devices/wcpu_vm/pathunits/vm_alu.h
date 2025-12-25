#ifndef __VM_CPU_PATH_ALU_HEADERS__
#define __VM_CPU_PATH_ALU_HEADERS__

#include "vm.h"
#include "vm_op.h"
#include "vm_txn.h"
#include <stdint.h>
#include <stdbool.h>

void set_alu_instructions(void);


vm_op_status_t vm_ALU_ADD_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_SUB_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_SUBU_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_AND_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_OR_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_XOR_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_SLL_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_SRL_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_SRA_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_DIV_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_REM_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_MUL_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_DIVU_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_MULHI_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_MULU_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_MULUS_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_NOT_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_CLT_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_CLE_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_CLTU_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_CLEU_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_CEQ_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_CNE_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_COR_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_CAND_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_CLTMV_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_CLEMV_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_CEQMV_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_CNEMV_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_MAX_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_MIN_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_SIEX_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_RAND_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);
vm_op_status_t vm_ALU_POPCNT_fn(vima_t *vm, vm_op_t *op, vm_txn_t *txn);

void vm_cpu_path_alu_init(vima_t *vm);

int64_t vm_alu_exec( char subflag, int64_t lane1, int64_t lane2, int64_t lane3, bool swap);



#endif
