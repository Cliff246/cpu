#ifndef __VM_CPU_PATH_JMP_HEADER__
#define __VM_CPU_PATH_JMP_HEADER__

#include "vm.h"
#include <stdint.h>


//uint64_t find_immedate_from_rel_table(vima_t *cpu, uint64_t address);
//void jump_to(vima_t *vm, uint64_t address);
//void jump_submit(vima_t *cpu, uint64_t subpath, int64_t lane1, int64_t lane2, int64_t lane3, char immf);

void vm_cpu_path_jmp_init(vima_t *vm);
void vm_cpu_path_jmp_pull(vima_t *vm);
void vm_cpu_path_jmp_push(vima_t *vm);
void vm_cpu_path_jmp_exec(vima_t *vm);



#endif
