#ifndef __VM_OP_HEADER__
#define __VM_OP_HEADER__

#include "isa.h"

#include <stdint.h>
#include <stdbool.h>

#define PATH_STRING_SIZE 10
#define OP_STRING_SIZE 20


#define MAX_PATH_OPS 128

typedef struct vm_operation vm_op_t;
typedef struct vm_transaction vm_txn_t;
typedef struct virtual_machine vima_t;

#define VM_OP_STATUS_LIST(X)\
	X(INVAL)				\
	X(START)				\
	X(LOADING)				\
	X(WAITING)				\
	X(TRANSITION)			\
	X(DONE)					\

#define VM_OP_STATUS_NAME(X) VM_OP_STATUS_ ## X
#define VM_OP_STATUS_ENUM(X) VM_OP_STATUS_NAME(X),

typedef enum vm_op_status
{
	VM_OP_STATUS_LIST(VM_OP_STATUS_ENUM)
}vm_op_status_t;

extern char *vm_op_status_str[];

typedef vm_op_status_t(*vm_run_op_fn)(vima_t *vm, vm_op_t *op, vm_txn_t *txn);


typedef struct vm_operation
{
	int subpath;
	vm_run_op_fn run;
	char string[OP_STRING_SIZE];
}vm_op_t;




typedef struct vm_path
{
	int path;
	char string[PATH_STRING_SIZE];
	vm_op_t ops[MAX_PATH_OPS];

}vm_path_t;




#endif
