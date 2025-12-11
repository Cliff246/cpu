#ifndef __VM_CPU_PATH_ALU_HEADERS__
#define __VM_CPU_PATH_ALU_HEADERS__

#include "vm.h"
#include <stdint.h>

void set_alu_instructions(void);



void vm_cpu_path_alu_init(vima_t *vm);
void vm_cpu_path_alu_pull(vima_t *vm);
void vm_cpu_path_alu_push(vima_t *vm);
void vm_cpu_path_alu_exec(vima_t *vm);



#endif
