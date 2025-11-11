
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

typedef struct code_desc
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
}code_desc_t;

typedef struct stack_reg
{
	//stack ptr,
	//stack frame
	uint64_t sp, sf;
	//stack code description
	uint64_t scd;
}stack_reg_t;

typedef struct interreg
{
	uint64_t rpc, ripc, gp, tp;
}inter_reg_t;

typedef struct instruction
{
	char path;
	char subpath;
	char rd ;
	char rs1 ;
	char rs2 ;
	char accflag;
	char selflag ;
	char reallocflag ;
	char immflag;
}inst_t;

typedef struct instbundle
{
	bool decoded;
	inst_t inst;
	uint64_t raw;
}instbundle_t;


typedef enum immtype
{
	IMM_UNKNOWN,
	IMM_FALSE,
	IMM_TRUE,
	IMM_REFERENCE,
	IMM_REALLOC,
	IMM_KEY
}immtype_t;

typedef struct immbundle
{
	immtype_t type;
	int64_t imm;
}immbundle_t;

typedef struct operation
{

	instbundle_t inst;
	immbundle_t imm;

}operation_t;


typedef struct regfile
{

	int64_t iregs[64];
	double fregs[64];

	stack_reg_t stack;
	inter_reg_t inter;
	code_desc_t desc;



}reg_file_t;

typedef struct
{
	//general regs
	reg_file_t user, kernal;
	pmode_t mode;

	bool has_jumped;

	stages_t stage;

	operation_t op;
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
