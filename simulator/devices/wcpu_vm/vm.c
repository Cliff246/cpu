#include "vm.h"
#include "vm_path.h"
#include "vm_regs.h"
#include "device_command.h"
#include "device_commons.h"
#include "device_command_impl.h"
#include "device_description.h"
#include "vm_op.h"
#include "vm_txn.h"

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>


static const WS_dev_vtable_t vtable =
{

	.init = device_vima_init,
	.print = device_vima_print,
	.read = device_vima_read,
	.send = device_vima_send,
	.update = device_vima_update,
	.stringfy = device_vima_stringfy,
	.cmd_commit = device_vima_commit,
};

static WS_dev_desc_t wcpu_vm_desc =
{
	.id = 0,
	.version = 0,

	.dev_name = "wcpu_vm",
	.dev_typeclass = "cpu",
	.dl_name = "sim_dev_wcpu_vm",
	.vtable = &vtable,
	.extra = NULL,
};



const WS_dev_desc_t *WS_get_dev_desc(void)
{
	static bool initialized = false;

	if(initialized == false)
	{
		p_hashtable_t hashtable = new_hash_table(0, WS_cmd_producer_free);


		wcpu_vm_desc.flag_table = hashtable;
		initialized = true;
	}


	return &wcpu_vm_desc;
}


void *device_vima_init(device_t *device)
{
	vima_t *vm = calloc(1, sizeof(vima_t));
	vm_init_bus(vm);


	vm_bus_port_id_t port_id = vm_bus_create_port(vm, VM_ADDR_KERNAL);

	vm_bus_evnt_t bus_event =
	{
		.evnt.load = {.addr = 0},
		.type = VM_IO_LOAD
	};
	vm_bus_hnd_t hnd =  vm_bus_put_evnt(vm, port_id, bus_event);

	vm_print_bus(vm);

	return vm;
	//return vm_init(1000);
}

void device_vima_update(device_t *dev)
{
	vima_t *vm = (vima_t *)dev->ptr;



	return;
}

bool device_vima_read(WS_dev_t *dev, WS_dev_msg_t *msg)
{
	vima_t *vm = (vima_t *)dev->ptr;
	for(int i = 0; i < BUS_MAX_HANDLES; ++i)
	{
		vm_hnd_t *hnd = &vm->bus.handles[i];
		if(hnd->used)
		{
			if(hnd->evnt.type == VM_IO_LOAD)
			{
				if(hnd->evnt.evnt.load.addr == msg->address)
				{
					hnd->internal.has_response = true;
					hnd->internal.response.resp.load_response.value = msg->value;

					return;
				}
			}
		}
	}


	return false;
}

bool device_vima_send(WS_dev_t *dev, WS_dev_msg_t **msg)
{
	vima_t *vm = (vima_t *)dev->ptr;


	for(int i = 0; i < BUS_MAX_HANDLES; ++i)
	{
		vm_hnd_t *hnd = &vm->bus.handles[i];
		if(hnd->used)
		{
			if(hnd->internal.has_send == false)
			{
				if(hnd->evnt.type == VM_IO_LOAD)
				{

					*msg = WS_device_message_create(dev->desc, dev->id, -1,DEVMSG_READ_SEND, hnd->evnt.evnt.load.addr, 0);
				}
				else
				{
					*msg = WS_device_message_create(dev->desc, dev->id, -1,DEVMSG_WRITE, hnd->evnt.evnt.store.addr, hnd->evnt.evnt.store.val);
				}
				hnd->internal.has_send = true;
				return true;
			}
		}
	}

	return false;
}

void device_vima_print(device_t *dev)
{
	vima_t *vm = (vima_t *)dev->ptr;

	return;
}

WS_dev_cmd_t *device_vima_stringfy(toklex_t *tl)
{
	WS_dev_cmd_t *cmd =  calloc(1, sizeof(WS_dev_cmd_t));

	cmd->type = &wcpu_vm_desc;
	cmd->used = false;
	cmd->collection = WS_cmd_collection_create(tl);


	return cmd;
}

void device_vima_commit(WS_dev_t *dev)
{
	vima_t *vm = (vima_t *)dev->ptr;


}

vima_t *vm_init(uint64_t memory_size)
{

}


int64_t vm_load(vima_t *vm, uint64_t address)
{

}

void vm_store(vima_t *vm, uint64_t address, int64_t data)
{
}

void vm_step(vima_t *vm)
{
	//vm_cpu_step(vm);
}


/*

void vm_setmemory(vima_t *vm, int64_t *memory, uint64_t size)
{
	//printf("side: %lld\n", size);
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
*/
