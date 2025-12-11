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
}