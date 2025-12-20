#ifndef __VM_CPU_PATH_ALU_HEADERS__
#define __VM_CPU_PATH_ALU_HEADERS__

#include "vima.h"
#include <stdint.h>
#include <stdbool.h>

void set_alu_instructions(void);



void vm_cpu_path_alu_init(vima_t *vm);

int64_t vm_cpu_path_alu_exec(vima_t *vm, char subflag, int64_t lane1, int64_t lane2, int64_t lane3, bool swap);



#endif
