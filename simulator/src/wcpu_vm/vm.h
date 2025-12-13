#ifndef __VIRTUAL_MACHINE_HEADER__
#define __VIRTUAL_MACHINE_HEADER__

#include <stdint.h>


#include "vm_cpu.h"







typedef struct
{
	int64_t *mem;
	uint64_t size;
}vm_mem_t;

typedef struct virtual_machine
{



	vm_mem_t mem;
	vm_cpu_t cpu;

}vima_t;

int64_t vm_load(vima_t *vm, uint64_t address);
void vm_store(vima_t *vm, uint64_t address, int64_t data);



vima_t *vm_init(uint64_t memory_size);
void vm_step(vima_t *vm);
void vm_setmemory(vima_t *vm, int64_t *memory, uint64_t size);
void vm_print_mem(vima_t *vm, uint64_t start, uint64_t stop);
#endif