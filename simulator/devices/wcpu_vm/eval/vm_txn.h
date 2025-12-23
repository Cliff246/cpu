#ifndef __VM_TRANSACTION_HEADER__
#define __VM_TRANSACTION_HEADER__

#include "vm_bus.h"
#include "commons.h"
#include "vm_op.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "isa.h"

#define VM_TXN_STATE_LIST(X)	\
	X(INVAL)					\
	X(FAULT)					\
	X(INIT)						\
	X(FETCH)					\
	X(DECODE)					\
	X(SOURCE)					\
	X(WIRE)						\
	X(EXEC)						\
	X(OUTPUT)					\
	X(RETIRE)					\
	X(DONE)						\

#define VM_TXN_STATE_NAME(X) VM_TXN_ ## X
#define VM_TXN_STATE_ENUM(X) VM_TXN_STATE_NAME(X),
typedef enum
{
	VM_TXN_STATE_LIST(VM_TXN_STATE_ENUM)
}vm_txn_state_t;

#define VM_TXN_STATE_COUNT (0 VM_TXN_STATE_LIST(COUNTER1D))

extern char *vm_txn_state_str[VM_TXN_STATE_COUNT];
typedef struct vm_operation vm_op_t;
typedef enum vm_op_status vm_op_status_t;
typedef struct virtual_machine vima_t;
//this will determine flags in the transaction can carry out
#define VM_TXN_FLAG_LIST(X)		\
	X(0, FETCH)					\
	X(1, DEPLOY)				\
	X(2, HANDLE)				\
	X(3, JUMPS)					\
	X(4, COUNTER)				\
	X(5, LOCAL)					\
	X(6, INTERUPT)				\


#define VM_TXN_FLAG_NAME(X) VM_TXN_FLAG_##X
#define VM_TXN_FLAG_MASK_NAME(X) VM_TXN_FLAG_##X ## _MASK
#define VM_TXN_FLAG_MASK_ENUM(X, Y) VM_TXN_FLAG_MASK_NAME(Y)= 1 << X,
#define VM_TXN_FLAG_TYPE_ENUM(X, Y) VM_TXN_FLAG_NAME(Y),
typedef enum
{
	VM_TXN_FLAG_LIST(VM_TXN_FLAG_MASK_ENUM)
}vm_txn_flag_mask_t;

typedef enum
{
	VM_TXN_FLAG_LIST(VM_TXN_FLAG_TYPE_ENUM)
}vm_txn_flag_t;

#define VM_TXN_FLAG_COUNT (0 VM_TXN_FLAG_LIST(COUNTER2D))

#define VM_TXN_FLAG_BITMASK_SIZE (VM_TXN_FLAG_COUNT / 8 + 1)

extern char *vm_txn_flag_str[VM_TXN_FLAG_COUNT];
extern vm_txn_flag_mask_t vm_txn_flag_mask_ary[VM_TXN_FLAG_COUNT];

#define VM_TXN_MAX_WORKING_SPACE 8

typedef struct vm_transaction
{
	//opcode
	vm_txn_state_t state;


	struct
	{
		inst_t op;
		uint64_t pc;
		uint64_t ipc;
		int64_t inst;
		int64_t imm;
	}op;

	struct
	{

		int64_t rs1, rs2, rs3, side;
	}srcs;

	struct
	{
		int64_t lane1, lane2, lane3;
		int dst;
		bool swap;
	}inp;

	struct
	{
		int64_t out;
		bool jumped;
	}out;



	struct
	{
		vm_bus_port_id_t port;
	}handle;

	struct
	{
		vm_op_status_t status;
		vm_op_t *op;
	}inflight;

	struct
	{
		int64_t it1, it2, it3, it4;
		uint64_t ut1, ut2, ut3, ut4;
	}private;

	struct
	{
		struct
		{
			uint64_t x, y;
			uint64_t lim;
		}iter;
		//temporary to a state
		//will be reset after the stage transitons
		int64_t tia, tib, tic, tid;
		uint64_t tua, tub, tuc, tud, tue, tuf, tug;

		int64_t working[VM_TXN_MAX_WORKING_SPACE];
	}local;

	char flags[VM_TXN_FLAG_BITMASK_SIZE];

	struct
	{
		vm_txn_state_t next;
	}next;


}vm_txn_t;

void vm_txn_reset_local(vm_txn_t *txn);
void vm_txn_state_inval(vima_t *vm, vm_txn_t *txn);
void vm_txn_state_fault(vima_t *vm, vm_txn_t *txn);
void vm_txn_state_init(vima_t *vm, vm_txn_t *txn);
void vm_txn_state_fetch(vima_t *vm, vm_txn_t *txn);
void vm_txn_state_decode(vima_t *vm, vm_txn_t *txn);
void vm_txn_state_source(vima_t *vm, vm_txn_t *txn);
void vm_txn_state_wire(vima_t *vm, vm_txn_t *txn);
void vm_txn_state_exec(vima_t *vm, vm_txn_t *txn);
void vm_txn_state_output(vima_t *vm, vm_txn_t *txn);
void vm_txn_state_retire(vima_t *vm, vm_txn_t *txn);
void vm_txn_state_done(vima_t *vm, vm_txn_t *txn);



static void (*txn_state_handle_ary[])(vima_t *vm, vm_txn_t *txn) =
{
	vm_txn_state_inval,
	vm_txn_state_fault,

	vm_txn_state_init,

	vm_txn_state_fetch,
	vm_txn_state_decode,
	vm_txn_state_source,
	vm_txn_state_wire,
	vm_txn_state_exec,
	vm_txn_state_output,
	vm_txn_state_retire,
	vm_txn_state_done,

};

void vm_txn_set_next_state(vm_txn_t *txn, vm_txn_state_t state);

bool vm_txn_check_flag(vm_txn_t *txn, vm_txn_flag_t flag);
void vm_txn_set_flag(vm_txn_t *txn, vm_txn_flag_t flag);

char *vm_txn_flag_to_str(vm_txn_flag_t flag);
char *vm_txn_flag_mask_to_str(vm_txn_flag_mask_t flag);

char *vm_txn_to_str(vm_txn_t *txn);


void vm_txn_switch_state(vima_t *vm, vm_txn_t *txn);
void vm_txn_advance(vima_t *vm, vm_txn_t *txn);
vm_txn_t *vm_txn_create(vima_t *vm);

void vm_txn_print_op(vm_txn_t *txn);
void vm_txn_print_src(vm_txn_t *txn);
void vm_txn_print_inp(vm_txn_t *txn);
void vm_txn_print_handle(vm_txn_t *txn);
void vm_txn_print_inflight(vm_txn_t *txn);
void vm_txn_print_private(vm_txn_t *txn);
void vm_txn_print_out(vm_txn_t *txn);
void vm_txn_print_next(vm_txn_t *txn);
void vm_txn_print_status(vm_txn_t *txn);

void vm_txn_print(vm_txn_t *txn);



#endif
