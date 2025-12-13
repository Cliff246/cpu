#ifndef __VM_CPU_PATH_SYS_HEADER__
#define __VM_CPU_PATH_SYS_HEADER__


#include "vm.h"
#include "flags.h"
#include <stdint.h>
#include <stdbool.h>

void vm_cpu_path_sys_init(vima_t *vm);
int64_t vm_cpu_path_sys_exec(vima_t *vm, char subflag, int64_t lane1, int64_t lane2, int64_t lane3, bool swap);

//void sys_set_cd_ptr(vima_t *vm, uint64_t rd);
//void sys_call_cd_ptr(vima_t *vm, uint64_t rd);

//void system_submit(vima_t *vm);


#endif