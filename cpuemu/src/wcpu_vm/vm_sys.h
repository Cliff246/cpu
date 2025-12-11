#ifndef __VM_CPU_PATH_SYS_HEADER__
#define __VM_CPU_PATH_SYS_HEADER__


#include "vm.h"
#include "flags.h"

void vm_cpu_path_sys_init(vima_t *vm);
void vm_cpu_path_sys_pull(vima_t *vm);
void vm_cpu_path_sys_push(vima_t *vm);
void vm_cpu_path_sys_exec(vima_t *vm);

//void sys_set_cd_ptr(vima_t *vm, uint64_t rd);
//void sys_call_cd_ptr(vima_t *vm, uint64_t rd);

//void system_submit(vima_t *vm);


#endif