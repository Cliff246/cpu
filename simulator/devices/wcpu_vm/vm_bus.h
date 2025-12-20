#ifndef __VM_BUS_HEADER__
#define __VM_BUS_HEADER__

typedef struct virtual_machine vima_t;

#include <stdint.h>
#include <stdbool.h>

#define BUS_MAX_TRANSFERS 100

typedef struct 
{
	bool valid;

	uint64_t address;
	int64_t data;
}vm_transfer_t;



typedef struct 
{
	vm_transfer_t transfers[BUS_MAX_TRANSFERS];
}vm_bus_t;

int vm_bus_active_transfers();
void vm_bus_add_transfer(vima_t *vm, uint64_t address);

vm_transfer_t vm_bus_has_data(vima_t *vm, uint64_t address);


#endif
