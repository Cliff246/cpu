#ifndef __COREUTILS__
#define __COREUTILS__

#include "core.h"


void push_scd(cpu_t *cpu, cd_frame_t frame);
cd_frame_t pop_scd(cpu_t *cpu);

cd_frame_t get_frame_from_address(cpu_t *cpu, uint64_t address);

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

cd_frame_t get_frame(pmode_t mode);
void set_frame(pmode_t mode, cd_frame_t frame);

#endif