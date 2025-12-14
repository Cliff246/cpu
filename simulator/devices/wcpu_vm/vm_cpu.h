
#ifndef __VM_CPU_HEADER__
#define __VM_CPU_HEADER__

#include <stdint.h>
#include <stdbool.h>

#include "vm_cpu_path.h"
#include "isa.h"

#define REG_ACC 63

#define BOOKMARKS_MAX 128


typedef enum
{
	VM_CPU_MODE_KERNAL,
	VM_CPU_MODE_USER
}vm_cpu_mode_t;

typedef enum
{
	VM_CPU_STAGE_FETCH,
	VM_CPU_STAGE_DECODE,
	VM_CPU_STAGE_EXECUTE,
	VM_CPU_STAGE_MEMORY,
	VM_CPU_STAGE_WRITEBACK,
}vm_cpu_stage_t;



typedef struct
{
    uint64_t inst_addr;
    uint64_t imm_addr;
	uint64_t cd_addr;
    int      valid;
}vm_cpu_bookmark_t;

typedef struct
{
	//code description ptr
	uint64_t cd_ptr;
	//pc_ptr
	uint64_t pc_base, pc_len;
	//ipc
	uint64_t ipc_base, ipc_len;
	//code description table
	uint64_t ct_base, ct_len;

	//frame pc, frame ipc
	uint64_t pc, ipc;
}vm_reg_cd_t;

typedef struct
{
	//stack ptr,
	//stack frame
	uint64_t sp, sfp;
	//stack code description
	uint64_t scd;
}vm_reg_stack_t;

typedef struct
{
	uint64_t rpc, ripc, gp, tp;
}vm_reg_inter_t;



typedef struct
{
	bool decoded;
	inst_t inst;
	uint64_t raw;
}vm_inst_bdl_t;


typedef enum
{
	IMM_UNKNOWN,
	IMM_FALSE,
	IMM_TRUE,
	IMM_REFERENCE,
	IMM_REALLOC,
	IMM_KEY
}vm_imm_type_t;

typedef struct vm_imm_bundle
{
	vm_imm_type_t type;
	int64_t imm;
}vm_imm_bdl_t;

typedef struct
{

	vm_inst_bdl_t inst;
	vm_imm_bdl_t imm;

}vm_op_t;

typedef struct
{

	int64_t regs[64];

	vm_reg_stack_t stack;
	vm_reg_inter_t inter;
	vm_reg_cd_t code_desc;



}vm_regfile_t;


typedef struct
{
	vm_cpu_path_type_t path;
	char subpath;
	bool sidechannel;
	int64_t lane1, lane2, lane3;
	int64_t dest;
}vm_cpu_pre_t;

typedef struct
{
	int64_t data;
}vm_cpu_post_t;

typedef struct vm_cpu
{
	//general regs
	vm_regfile_t user;
	vm_cpu_mode_t mode;

	bool has_jumped;

	vm_cpu_stage_t stage;

	vm_op_t op;
	//current input 1, 2, 3
	//current output

	vm_cpu_pre_t pre;
	vm_cpu_post_t post;

	vm_cpu_bookmark_t bookmarks[BOOKMARKS_MAX];
	bool stop;
	bool program_over;

}vm_cpu_t;












//step the cpu forward
void vm_cpu_step(vima_t *vm);
void vm_cpu_set_inital_cd(vima_t *vm);
void vm_cpu_print_regs(vima_t *vm);













#endif
