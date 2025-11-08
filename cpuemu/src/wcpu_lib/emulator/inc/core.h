
#ifndef __CORE__
#define __CORE__

#include <stdint.h>
#include <stdbool.h>

#include "common.h"
#include "cpumemory.h"
#include "palu.h"

#define BOOKMARKS_MAX 128


typedef enum
{
	KERNAL,
	USER
}pmode_t;

typedef enum
{
	FETCH,
	DECODE,
	EXECUTE,
	MEMORY,
	WRITEBACK,
}stages_t;



typedef struct
{
    uint64_t inst_addr;
    uint64_t imm_addr;
	uint64_t cd_addr;
    int      valid;
} bookmark_t;

typedef struct cd_frame
{
	//code description ptr
	uint64_t cd_ptr;
	//pc_ptr
	uint64_t pc_ptr, pc_len;
	//ipc
	uint64_t ipc_ptr, ipc_len;
	//code description table
	uint64_t ct_ptr, ct_len;

	//frame pc, frame ipc
	uint64_t pc, ipc;
}cd_frame_t;

typedef struct regstack
{
	//stack ptr,
	//stack frame
	uint64_t sp, sf;
	//stack code description
	uint64_t scd;
}regstack_t;

typedef struct interreg
{
	uint64_t rpc, ripc, gp, tp;
}interreg_t;

typedef struct
{

}live_wires_t;


typedef struct instruction
{
	char  path;
	char  subpath;
	char  rd ;
	char  rs1 ;
	char  rs2 ;
	char  selflag ;
	char  reallocflag ;
	char  immflag;
	int64_t imm;
}inst_t;



typedef struct
{


	//general regs
	int64_t regs[64];

	int64_t kregs[64];

	//instruction pc and imm_pc
	//kernal stack pointer and stack frame
	regstack_t u_stack, k_stack;
	//kernal/user mode return pc and ipc
	//kernal/user mode general ptr
	//kernal/user mode thread ptr
	interreg_t u_ireg, k_ireg;
	//cpu mode
	pmode_t mode;
	//kernal and usr stack code description ptr

	//
	cd_frame_t u_cd, k_cd;

	//kernal frame data table ptr and length
	uint64_t k_fdt_ptr, k_fdt_len;
	//user frame data table ptr and len
	uint64_t u_fdt_ptr, u_fdt_len;

	//kernal function table ptr and length
	uint64_t k_fn_table_ptr, k_fn_table_len;
	//user function table ptr
	uint64_t u_fn_table_ptr, u_fn_table_len;

	//user mode frame
	uint64_t u_mode_frame;
	//book marks filled;


	bool has_jumped;

	stages_t stage;

	int64_t curimm;
	int32_t curins;
	inst_t inst;
	//current input 1, 2, 3
	int64_t ci1, ci2, ci3;
	//current output
	int64_t co;

	bookmark_t bookmarks[BOOKMARKS_MAX];

} weirdo_cpu_t, cpu_t;



void print_cpu_state(cpu_t *cpu);



//create cpu
cpu_t *create_cpu(void);


//step the cpu forward
void step_cpu(void);
void startup_cpu(void);

void fetch_cpu(void);
void decode_cpu(void);
void execute_cpu(void);
void memory_cpu(void);
void writeback_cpu(void);





typedef enum
{
	ldst_mem,
	ldst_fake
}ldst_type;


//decode instruction form int32_t
inst_t decode_inst(int32_t code);
int32_t encode_inst(inst_t *inst);
uint64_t encode(uint64_t path, uint64_t subpath, uint64_t rd, uint64_t rs1, uint64_t rs2, uint64_t aux, uint64_t immf);

uint64_t load(uint64_t address);

void store(uint64_t address, int64_t value);
//register get


void print_inst(inst_t *inst);
void print_regs(void);

typedef struct
{
	memory_t *mem;
	cpu_t *cpu;
	alu_t *alu;
}components_t;


void init_components(void);
void free_components(void);
extern components_t components;

#endif
