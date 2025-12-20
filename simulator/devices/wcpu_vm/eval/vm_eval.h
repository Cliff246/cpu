#ifndef __VM_CPU_EVAL_HEADER__
#define __VM_CPU_EVAL_HEADER__

typedef struct virtual_machine vima_t;

#include "isa.h"
#include <stdint.h>
#include "vm_bus.h"
#include <stdbool.h>



typedef enum 
{
	VM_EVAL_STATE_INIT,
	VM_EVAL_STATE_LOAD,
	VM_EVAL_STATE_EXEC,
	VM_EVAL_STATE_DONE,
}vm_eval_state_t;

typedef enum 
{
	VM_EVAL_STAGE_FETCH,
	VM_EVAL_STAGE_AGGREGATE,
	VM_EVAL_STAGE_EXECUTE,
	VM_EVAL_STAGE_FINISH,
}vm_eval_stage_t;

typedef struct 
{
	enum 
	{
		VM_EVAL_FETCH_INSTR,
		VM_EVAL_FETCH_IMM,
	}cycle;
	bool instr_even_or_odd;
	int64_t instr;
	uint64_t instr_addr;
	int64_t imm;	
	uint64_t imm_addr;

	
	

}vm_eval_stage_fetch_t;

typedef struct 
{
	enum 
	{
		VM_EVAL_AGGREGATE_REALLOC,
	}cycle;
	inst_t inst;
	int dest;
	int64_t sidestream;
	int64_t lane1;
	int64_t lane2;
	int64_t lane3;
	int64_t realloc;
}vm_eval_stage_aggregate_t;

#define VM_EVAL_BUFFER_SIZE 100 


typedef struct 
{
	enum 
	{
		VM_EVAL_EXECUTE_MEMORY,
		VM_EVAL_EXECUTE_JUMP,
		VM_EVAL_EXECUTE_CD_SWAP,
		VM_EVAL_EXECUTE_NONE,
	}cycle;
	int64_t current;
	int64_t limit;
	int64_t result;
	
}vm_eval_stage_execute_t;

typedef struct 
{
	enum 
	{
		VM_EVAL_FINISH_STORE
	}cycle;
}vm_eval_stage_finish_t;

typedef struct 
{
	vm_eval_state_t state;
	vm_eval_stage_t stage;

	vm_eval_stage_fetch_t fetch;
	vm_eval_stage_aggregate_t aggregate;
	vm_eval_stage_execute_t exec;
	vm_eval_stage_finish_t finish;
	
	bool waiting;
	//vm_transfer_t buffer[VM_EVAL_BUFFER_SIZE];




}vm_eval_live_t;


static void advance_eval_live_state(vm_eval_live_t *live);
static void reset_eval_live_state(vm_eval_live_t *live);

void vm_eval(vima_t *vm);




#endif
