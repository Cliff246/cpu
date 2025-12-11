#include "vm.h"
#include "vm_cpu_path.h"
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>


vima_t *vm_init(uint64_t memory_size)
{
	vima_t *vm = calloc(1, sizeof(vima_t));
	assert(vm);

	for(int i = 0; i < VM_CPU_PATH_COUNT; ++i)
	{
		vm_cpu_path_fn[i].init(vm);
	}

	vm->mem.mem = calloc(memory_size, sizeof(int64_t));
	vm->mem.size = memory_size;

	return vm;
}


int64_t vm_load(vima_t *vm, uint64_t address)
{
	assert(address < vm->mem.size);
	return vm->mem.mem[address];
}

void vm_store(vima_t *vm, uint64_t address, int64_t data)
{
	assert(address < vm->mem.size);
	vm->mem.mem[address] = data;
}

void vm_step(vima_t *vm)
{
	
}