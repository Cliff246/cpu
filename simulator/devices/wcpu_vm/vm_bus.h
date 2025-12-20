#ifndef __VM_BUS_HEADER__
#define __VM_BUS_HEADER__

typedef struct virtual_machine vima_t;

#include <stdint.h>
#include <stdbool.h>

#define BUS_MAX_TRANSFERS 100

typedef int vm_bus_hnd_t;
typedef int vm_bus_port_id_t;

typedef enum 
{
	VM_ADDR_INVAL,
	VM_ADDR_USER,
	VM_ADDR_KERNAL,
}vm_addr_mode_t;


typedef enum 
{
	VM_IO_INVAL,
	VM_IO_LOAD,
	VM_IO_STORE,
}vm_io_type_t;

typedef struct 
{
	uint64_t addr;
	int64_t val;
}vm_bus_evnt_store_t;

typedef struct 
{
	uint64_t addr;
}vm_bus_evnt_load_t;


typedef struct 
{
	vm_io_type_t type;
	union 
	{
		vm_bus_evnt_load_t load;
		vm_bus_evnt_store_t store;
	}evnt;
}vm_bus_evnt_t;



typedef struct
{
	vm_io_type_t type;
	union 
	{
		struct
		{
			int64_t value;
		}load_response;

	}resp;	
}vm_bus_response_t;


typedef struct
{
	int counter;
	
	//temp
	vm_bus_response_t response;
	

}vm_hnd_internal_t;

typedef struct 
{
	//the inital event
	vm_bus_evnt_t evnt;
	//the id of the event
	vm_bus_hnd_t hnd;
	//the internals
	vm_hnd_internal_t internal;
	//the id of the evnt owner
	vm_bus_port_id_t id;
}vm_hnd_t;

typedef struct 
{
	vm_bus_port_id_t id;
	//count of the handles
	int handles;
	//the mode tranlation done
	vm_addr_mode_t mode;
}vm_bus_port_t;

typedef struct 
{
	vm_bus_port_t *ports;
	uint64_t ports_size;
	vm_hnd_t *handles;
	uint64_t handles_size;

}vm_bus_t;


vm_bus_port_id_t vm_bus_create_port(vima_t *vm, vm_addr_mode_t mode); 
//puts event and submits it to the device system
vm_bus_hnd_t vm_bus_put_evnt(vima_t *vm, vm_bus_port_id_t port, vm_bus_evnt_t evnt);
//polls event with the handle recieved
bool vm_bus_poll_evnt(vima_t *vm, vm_bus_port_id_t port, vm_bus_hnd_t hnd); 
//response of event
vm_bus_response_t vm_bus_pull_evnt(vima_t *vm, vm_bus_port_id_t port, vm_bus_hnd_t hnd);

bool vm_bus_free_port(vima_t *vm, vm_bus_port_id_t port); 



#endif
