
#ifndef __VM_CPU_HEADER__
#define __VM_CPU_HEADER__

#include <stdint.h>
#include <stdbool.h>

#include "isa.h"

#define REG_ACC 63

#define BOOKMARKS_MAX 128

typedef enum
{
	VM_CPU_MODE_KERNAL,
	VM_CPU_MODE_USER
}vm_cpu_mode_t;




typedef struct
{
    uint64_t inst_addr;
    uint64_t imm_addr;
	uint64_t cd_addr;
    int      valid;
}vm_bookmark_t;

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

	int64_t regs[64];

	vm_reg_stack_t stack;
	vm_reg_inter_t inter;
	vm_reg_cd_t code_desc;



}vm_regfile_t;





void vm_cpu_print_regs(vima_t *vm);
























#endif
