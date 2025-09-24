
#ifndef __CORE__
#define __CORE__

#include <stdint.h>
#include <stdbool.h>

#include "memory.h"
#include "palu.h"


#define BOOKMARKS_MAX 128

#define SETBIT(n, k) (1 | (n << k))
#define CLRBIT(n, k) (n & ~(1 << k))
#define GETBIT(n, k) (1 & (n >> k))


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
    int      valid;
} bookmark_t;

typedef struct
{


	//general regs
	int64_t regs[32];

	int64_t kregs[32];

	//instruction pc and imm_pc
	uint64_t inst_pc, imm_pc;
	uint64_t inst_pc_limit, imm_pc_limit;
	uint64_t inst_pc_start, imm_pc_start;
	uint64_t swap_inst_pc, swap_imm_pc;
	//kernal stack pointer and stack frame
	uint64_t k_sp, k_sf, sf, sp;
	//cpu mode
	pmode_t mode;
	//kernal and usr stack code description ptr
	uint64_t k_cd_ptr, u_cd_ptr;
	uint64_t k_cd_len, u_cd_len;
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

	stages_t stage;

	int64_t curimm;
	int32_t curins;
	//current input 1, 2, 3
	int64_t ci1, ci2, ci3;
	//current output
	int64_t co;

	bookmark_t bookmarks[BOOKMARKS_MAX];

} weirdo_cpu_t, cpu_t;



void print_cpu_state(cpu_t *cpu);


typedef struct instruction
{
	uint32_t path : 4;
	uint32_t subpath : 8;
	uint32_t rd : 5;
	uint32_t rs1 : 5;
	uint32_t rs2 : 5;
	uint32_t aux : 3;
	uint32_t immflag;
}inst_t;



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

//get the right address for memory
uint64_t address(uint64_t addr);

typedef enum
{
	ldst_mem,
	ldst_fake
}ldst_type;

uint64_t load(uint64_t address);
void store(uint64_t address, int64_t value);
//decode instruction form int32_t
inst_t decode_inst(int32_t code);
int32_t encode_inst(inst_t *inst);
uint64_t encode(uint64_t path, uint64_t subpath, uint64_t rd, uint64_t rs1, uint64_t rs2, uint64_t aux, uint64_t immf);

//register get
int64_t get_reg(int reg);
//register set
void set_reg(int reg, int64_t content);

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
void print_bin(long bin, char len, bool newline);

void print_hex(char hex, bool newline);
void print_str_hex(char *str, bool newline);
void reverse(char *ary);

#endif
