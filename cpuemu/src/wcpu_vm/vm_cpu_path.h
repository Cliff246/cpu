#ifndef __VM_CPU_PATH_HEADER__
#define __VM_CPU_PATH_HEADER__

#include <stdint.h>
typedef struct virtual_machine vima_t;

#define VM_CPU_PATH_LIST(X)\
	X(alu)				   \
	X(mem)				   \
	X(jmp)				   \
	X(sys)				   \
//	X(fpu)				   \
//	X(atm)				   \
//	X(vec)				   \

#define VM_CPU_PATH_NAME(X) path_ ## X
#define VM_CPU_PATH_ENUM(X) VM_CPU_PATH_NAME(X),

#define VM_CPU_PATH_COUNTER(X) + 1
#define VM_CPU_PATH_COUNT (0 VM_CPU_PATH_LIST(VM_CPU_PATH_COUNTER))

typedef enum
{
	VM_CPU_PATH_LIST(VM_CPU_PATH_ENUM)
}vm_cpu_path_type_t;

typedef void (*vm_cpu_path_pull_fn)(vima_t *vm);
typedef void (*vm_cpu_path_exec_fn)(vima_t *vm);
typedef void (*vm_cpu_path_push_fn)(vima_t *vm);
typedef void (*vm_cpu_path_init_fn)(vima_t *vm);


typedef struct
{
	vm_cpu_path_exec_fn exec;
	vm_cpu_path_pull_fn pull;
	vm_cpu_path_push_fn push;
	vm_cpu_path_init_fn init;

}vm_cpu_path_vtable_t;

extern vm_cpu_path_vtable_t vm_cpu_path_fn[VM_CPU_PATH_COUNT];




#endif