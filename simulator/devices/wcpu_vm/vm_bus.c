#include "vm_bus.h"
#include "vm.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

//o(n) haha... fuck
vm_port_t *vm_find_port(vima_t *vm, vm_bus_port_id_t id)
{
	vm_bus_t *bus = &vm->bus;
	assert(id >= 0 && id < BUS_MAX_PORTS);
	return &bus->ports[id];

}

void vm_bus_free_handle(vima_t *vm, vm_bus_hnd_t hnd)
{
	memset(&vm->bus.handles[hnd], 0, sizeof(vm_hnd_t));
	vm->bus.handles[hnd].hnd = hnd;
}


vm_bus_port_id_t vm_bus_create_port(vima_t *vm, vm_addr_mode_t mode)
{
	vm_bus_t *bus = &vm->bus;

	for(int i = 0; i < BUS_MAX_PORTS; ++i)
	{
		vm_port_t *port = &bus->ports[i];
		if(port->used == false)
		{
			port->mode = mode;
			port->used = true;
			port->handles = 0;
			return i;
		}
	}
	assert(0);
	return -1;


}


vm_bus_hnd_t vm_bus_put_evnt(vima_t *vm, vm_bus_port_id_t port, vm_bus_evnt_t evnt)
{
	vm_bus_t *bus = &vm->bus;


 	vm_port_t *found =  vm_find_port(vm, port);
	assert(found);

	if(!found)
	{
		return -1;
	}

	for(int i = 0; i < BUS_MAX_HANDLES; ++i)
	{
		vm_hnd_t *hnd = &bus->handles[i];
		if(hnd->used == false)
		{
			hnd-> port = port;
			hnd->evnt = evnt;
			hnd->used = true;
			memset(&hnd->internal, 0, sizeof(vm_hnd_internal_t));
			found->handles++;

			return hnd->hnd;
		}
	}

	assert(0);
	return -1;

}
//polls event with the handle recieved
bool vm_bus_poll_evnt(vima_t *vm, vm_bus_port_id_t port, vm_bus_hnd_t hnd)
{
	vm_bus_t *bus = &vm->bus;

	vm_port_t *found = vm_find_port(vm, port);
	if(!found)
	{
		return -1;
	}
	if(found->handles <= 0)
	{
		return false;
	}
	if(hnd < 0 || hnd > BUS_MAX_HANDLES)
		return false;

	vm_hnd_t *handle = &bus->handles[hnd];

	if(handle->port == port && handle->used)
	{
		if(handle->internal.has_response)
			return true;
		else
			return false;
	}
	else
		return false;
}
//response of event
bool vm_bus_pull_evnt(vima_t *vm, vm_bus_port_id_t port, vm_bus_hnd_t hnd, vm_bus_response_t *response)
{
	vm_bus_t *bus = &vm->bus;

	vm_port_t *found = vm_find_port(vm, port);
	vm_bus_response_t resp = {0};
	if(!found)
	{
		return false;
	}
	if(found->handles <= 0)
	{
		return  false;
	}
	if(hnd < 0 || hnd > BUS_MAX_HANDLES)
		return false;

	vm_hnd_t *handle = &bus->handles[hnd];

	if(handle->port == port && handle->used)
	{
		if(handle->internal.has_response)
		{
			*response = handle->internal.response;
			found->handles--;
			vm_bus_free_handle(vm, hnd);
			return true;
		}
		else
			return false;
	}
	assert(0);

}


bool vm_bus_free_port(vima_t *vm, vm_bus_port_id_t port)
{
	memset(&vm->bus.ports[port], 0, sizeof(vm_port_t));
	vm->bus.ports[port].id = port;
}


void vm_init_bus(vima_t *vm)
{
	vm_bus_t *bus = &vm->bus;
	for(int a = 0; a < BUS_MAX_PORTS; ++a)
	{
		bus->ports[a].id = a;

	}

	for(int b = 0; b < BUS_MAX_HANDLES; ++b)
	{
		bus->handles[b].hnd = b;
		bus->handles[b].port = -1;
	}
}



void vm_print_bus(vima_t *vm)
{
	vm_bus_t *bus = &vm->bus;

	for(int a = 0; a < BUS_MAX_PORTS; ++a)
	{
		vm_print_port(&bus->ports[a]);

	}

	for(int b = 0; b < BUS_MAX_HANDLES; ++b)
	{
		vm_print_hnd(&bus->handles[b]);

	}
}



void vm_print_hnd(vm_hnd_t *hnd)
{
	printf("\n hnd_id %d: hnd_port: %d\n hnd_used:%d\n", hnd->hnd, hnd->port, hnd->used);
	vm_print_bus_event(&hnd->evnt);
}

void vm_print_port(vm_port_t *port)
{
	printf("\n port_id: %d\n port_handles: %d\n port_used: %d\n port mode: %d\n", port->id, port->handles, port->used, port->mode);
}

static void vm_print_bus_event_store(vm_bus_evnt_store_t *store)
{
	printf("store[%llu] = %lld\n", store->addr, store->val);
}
static void vm_print_bus_event_load(vm_bus_evnt_load_t *load)
{
	printf("load [%llu] \n", load->addr);
}

void vm_print_bus_event(vm_bus_evnt_t *event)
{
	switch(event->type)
	{
		case VM_IO_LOAD:
			vm_print_bus_event_load(&event->evnt.load);
			break;
		case VM_IO_STORE:
			vm_print_bus_event_store(&event->evnt.store);
			break;
		default:
			break;
	}
}
