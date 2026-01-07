#include "vm.h"
#include "vm_path.h"
#include "vm_regs.h"
#include "device_command.h"
#include "device_commons.h"
#include "device_command_impl.h"
#include "module_description.h"
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
		p_hashtable_t hashtable = new_hash_table(WS_DEV_FLAG_DEFAULT_COUNT, WS_cmd_producer_free);


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
	vm->control.port = port_id;

	vm_bus_evnt_t bus_event =
	{
		.evnt.load = {.addr = (vm->control.cd_count++)},
		.type = VM_IO_LOAD
	};
	vm->control.hnd = vm_bus_put_evnt(vm, port_id, bus_event);
	vm->control.load_cd = true;
	return vm;
	//return vm_init(1000);
}

void device_vima_update(device_t *dev)
{
	vima_t *vm = (vima_t *)dev->ptr;

	if(vm->control.load_cd == true && vm->control.wait_until_inst_end == false)
	{
		if(vm_bus_poll_evnt(vm, vm->control.port, vm->control.hnd))
		{
			vm_bus_response_t resp;
			vm_bus_pull_evnt(vm, vm->control.port, vm->control.hnd, &resp);
			//printf("%d %d %d\n", resp.resp.load_response.value, vm->control.cd_count, vm->control.cd_base);
			switch(vm->control.cd_count)
			{
				case 1:
					vm->regs.code_desc.ct_base = resp.resp.load_response.value;
					break;
				case 2:
					vm->regs.code_desc.ct_len = resp.resp.load_response.value;
					break;
				case 3:
					vm->regs.code_desc.pc_base = resp.resp.load_response.value;
					break;
				case 4:
					vm->regs.code_desc.pc_len = resp.resp.load_response.value;
					break;
				case 5:
					vm->regs.code_desc.ipc_base = resp.resp.load_response.value;
					break;
				case 6:
					vm->regs.code_desc.ipc_len = resp.resp.load_response.value;
					break;
			}

			if(vm->control.cd_count >= 6)
			{
				vm->control.load_cd = false;
				vm->control.new_inst = true;
				vm->control.wait_until_inst_end = true;
			}
			else
			{
				vm_bus_evnt_t bus_event =
				{
					.evnt.load = {.addr = vm->control.cd_base + vm->control.cd_count++},
					.type = VM_IO_LOAD
				};
				vm->control.hnd = vm_bus_put_evnt(vm, vm->control.port, bus_event);
			}

		}
	}
	else
	{
		if(vm->control.new_inst)
		{
			if(vm->control.load_cd == true)
			{
				vm->control.wait_until_inst_end = false;
				vm->regs.code_desc.ipc = 0;
				vm->regs.code_desc.pc = 0;
				vm_bus_evnt_t bus_event =
				{
					.evnt.load = {.addr = (vm->control.cd_base + vm->control.cd_count++)},
					.type = VM_IO_LOAD
				};
				vm->control.hnd = vm_bus_put_evnt(vm, vm->control.port, bus_event);
			}
			else
			{
				if(vm->txn != NULL)
				{
					free(vm->txn);
					vm->txn = NULL;
				}
				vm->control.new_inst = false;

				vm->txn = vm_txn_create(vm);

			}


		}
		else
		{
			//vm_cpu_print_regs(vm);

			vm_txn_advance(vm, vm->txn);
			//vm_txn_print_inflight(vm->txn);
			//printf("\n");
		}
	}
	//printf("%d\n", vm->regs.stack.sp);

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

					return true;
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
					*msg = WS_device_message_create(&wcpu_vm_desc, 0, -1, DEVMSG_READ_SEND, hnd->evnt.evnt.load.addr, 0);
				}
				else
				{
					//printf("write\n");
					*msg = WS_device_message_create(&wcpu_vm_desc, 0, -1,DEVMSG_WRITE, hnd->evnt.evnt.store.addr, hnd->evnt.evnt.store.val);
					vm_bus_free_handle(vm, hnd->hnd);
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
	vm_cpu_print_regs(vm);
	printf("\n");
	return;
}



void device_vima_commit(WS_dev_t *dev)
{
	vima_t *vm = (vima_t *)dev->ptr;


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
