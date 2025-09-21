
#ifndef __CORE__
#define __CORE__

#include <stdint.h>
#include "memory.h"
#include "alu.h"
#define IMM_BOOKMARKS 256



typedef enum 
{
	KERNAL,
	USER
}pmode_t;



typedef struct 
{

	uint64_t pc, inst_pc;
}bookmark_t;

typedef struct
{
	//general regs
	int64_t regs[32];

	//instruction pc and imm_pc
	uint64_t inst_pc, imm_pc;
	uint64_t swap_inst_pc, swap_imm_pc;
	//kernal stack pointer and stack frame
	uint64_t k_sp, k_sf, sf, sp;
	//cpu mode
	pmode_t mode;
	//kernal and usr stack code description ptr
	uint64_t k_cd_ptr, u_cd_ptr;
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
	bookmark_t cpu_bookmark[IMM_BOOKMARKS];


		
} weirdo_cpu_t, cpu_t;






typedef struct instruction
{
	uint32_t path : 4;
	uint32_t subpath : 8;
	uint32_t rd : 5;
	uint32_t rs1 : 5;
	uint32_t rs2 : 5;
	uint32_t aux : 3;
	uint32_t immflag : 2;
}inst_t;



//create cpu
cpu_t *create_cpu(void);


//step the cpu forward
void step_cpu(void);


//get the right address for memory
uint64_t address(uint64_t addr); 


//decode instruction form int32_t
inst_t decode_inst(int32_t code);

//register get 
int64_t get_reg(int reg);
//register set
void set_reg(int reg, int64_t content);



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
