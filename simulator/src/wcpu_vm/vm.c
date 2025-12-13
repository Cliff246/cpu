#include "vm.h"
#include "vm_cpu_path.h"
#include "vm_cpu.h"
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>


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
	vm_cpu_step(vm);
}

void vm_setmemory(vima_t *vm, int64_t *memory, uint64_t size)
{
	printf("side: %d\n", size);
	vm->mem.mem = calloc(size, sizeof(int64_t));
	vm->mem.size = size;

	for(int i = 0; i < size; ++i)
	{
		vm->mem.mem[i] = memory[i];
	}

}
void vm_print_mem(vima_t *vm, uint64_t start, uint64_t stop)
{
	if(stop < start)
		return;
	uint64_t diff = stop - start;
	if(vm->mem.size < stop)
	{
		stop = vm->mem.size;
	}
	for(uint64_t i = start; i < stop; ++i)
	{
		printf("%4lld 0x%.8llx %llu\n", i, vm->mem.mem[i], vm->mem.mem[i]);
	}

}