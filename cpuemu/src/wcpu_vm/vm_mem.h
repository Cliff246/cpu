#ifndef __VM_CPU_PATH_MEMORY_HEADER__
#define __VM_CPU_PATH_MEMORY_HEADER__

#include "vm.h"

//void memory_submit(vima_t *vm, char subpath, int64_t lane1, int64_t lane2, int64_t lane3, char immflag );
void vm_cpu_path_mem_init(vima_t *vm);
void vm_cpu_path_mem_pull(vima_t *vm);
void vm_cpu_path_mem_push(vima_t *vm);
void vm_cpu_path_mem_exec(vima_t *vm);


#endif
