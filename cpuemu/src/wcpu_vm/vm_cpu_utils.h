#ifndef __VM_CPU_UTILS_HEADER__
#define __VM_CPU_UTILS_HEADER__

#include "vm_cpu.h"
#include "vm.h"
#include <stdint.h>


int64_t vm_cpu_get_lane1(vima_t *vm);
int64_t vm_cpu_get_lane2(vima_t *vm);
int64_t vm_cpu_get_lane3(vima_t *vm);


/*
inst_t get_inst_from_op(operation_t *op);



void push_scd(cpu_t *cpu, code_desc_t desc);
code_desc_t pop_scd(cpu_t *cpu);
code_desc_t get_desc_from_address(cpu_t *cpu, uint64_t address);


reg_file_t get_current_file(cpu_t *cpu);
reg_file_t get_mode_file(cpu_t *cpu, pmode_t mode);


operation_t fill_operation(uint32_t inst, int64_t imm);

void decode_operation(operation_t *op);


void set_current_file(cpu_t *cpu, reg_file_t file);

void set_mode_file(cpu_t *cpu, reg_file_t file, pmode_t mode);


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


uint32_t get_inst_at_pc_address(uint64_t address);

//get the right address for memory
uint64_t address(uint64_t addr);

int64_t get_reg(int reg);
//register set
void set_reg(int reg, int64_t content);

code_desc_t get_desc(pmode_t mode);
void set_desc(pmode_t mode, code_desc_t desc);

*/

#endif
