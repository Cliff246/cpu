#ifndef __VM_CPU_UTILS_HEADER__
#define __VM_CPU_UTILS_HEADER__



#include "vm.h"
#include <stdint.h>


int64_t vm_cpu_get_lane1(vima_t *vm);
int64_t vm_cpu_get_lane2(vima_t *vm);
int64_t vm_cpu_get_lane3(vima_t *vm);





void vm_push_scd(vima_t *vm, vm_reg_cd_t desc);
vm_reg_cd_t vm_pop_scd(vima_t *vm);
vm_reg_cd_t vm_get_cd_from_address(vima_t *vm, uint64_t address);

vm_reg_cd_t vm_get_desc(vima_t *vm);
void vm_set_desc(vima_t *vm, vm_reg_cd_t desc);

uint64_t vm_get_ipc(vima_t *vm);
uint64_t vm_get_pc(vima_t *vm);

void vm_set_ipc(vima_t *vm, uint64_t set);
void vm_set_pc(vima_t *vm, uint64_t set);
void vm_inc_ipc(vima_t *vm, uint64_t inc);
void vm_inc_pc(vima_t *vm, uint64_t inc);


uint64_t vm_get_pc_base(vima_t *vm);
uint64_t vm_get_ipc_base(vima_t *vm);


uint64_t vm_get_pc_len(vima_t *vm);
uint64_t vm_get_ipc_len(vima_t *vm);


void vm_set_sfp(vima_t *vm,uint64_t set);
uint64_t vm_get_sfp(vima_t *vm);
void vm_set_sp(vima_t *vm,uint64_t set);
uint64_t vm_get_sp(vima_t *vm);

uint64_t vm_dec_sp(vima_t *vm, uint64_t degree);
uint64_t vm_inc_sp(vima_t *vm, uint64_t degree);


uint32_t vm_get_inst_at_pc_address(vima_t *vm,uint64_t address);

//get the right address for memory
uint64_t vm_address(vima_t *vm,uint64_t addr);

int64_t vm_cpu_get_reg(vima_t *vm,int reg);
//register set
void vm_cpu_set_reg(vima_t *vm, int reg, int64_t content);




#endif
