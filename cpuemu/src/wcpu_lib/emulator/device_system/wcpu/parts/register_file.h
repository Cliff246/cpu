#ifndef __WCPU_PART_REGISTER_FILE_HEADER__
#define __WCPU_PART_REGISTER_FILE_HEADER__

#include <stdint.h>
#include "wcpu_part_ptr.h"

#define BOOKMARKS_MAX 128
#define REG_ACC 63



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

typedef struct
{
    uint64_t inst_addr;
    uint64_t imm_addr;
	uint64_t cd_addr;
    int      valid;
} bookmark_t;

typedef struct
{
	bookmark_t bookmarks[BOOKMARKS_MAX];
}bookmark_file_t;


typedef struct wcpu_part_regfile
{

	int64_t iregs[64];
	double fregs[64];

	stack_reg_t stack;
	inter_reg_t inter;
	code_desc_t desc;

	bookmark_file_t bookmarks;

}regfile_t;


part_ptr_t wcpu_regfile_generate(void);
void wcpu_regfile_update(part_t *regfile);


void print_regs(void);

int64_t get_reg(int reg);
//register set
void set_reg(int reg, int64_t content);



void set_current_file(regfile_t file);

void set_mode_file(regfile_t file);


uint64_t get_ipc(void);
uint64_t get_pc(void);

void set_ipc(uint64_t set);
void set_pc(uint64_t set);

void inc_ipc(uint64_t inc);
void inc_pc(uint64_t inc);


uint64_t get_pc_offset(void);
uint64_t get_ipc_offset(void);


uint64_t get_pc_len(void);
uint64_t get_ipc_len(void);


void set_sfp(uint64_t set);
uint64_t get_sfp(void);
void set_sp(uint64_t set);
uint64_t get_sp(void);

uint64_t dec_sp(uint64_t degree);
uint64_t inc_sp(uint64_t degree);



#endif