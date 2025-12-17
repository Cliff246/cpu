#ifndef __VIRTUAL_MACHINE_HEADER__
#define __VIRTUAL_MACHINE_HEADER__

#include <stdint.h>


#include "vm_cpu.h"

#include "device.h"
#include "token.h"
#include "device_message.h"
#include "device_command.h"



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



void *device_vima_init(device_t *device);

void device_vima_update(device_t *dev);
bool device_vima_read(WS_dev_t *dev, WS_dev_msg_t *msg);
bool device_vima_send(WS_dev_t *dev, WS_dev_msg_t **msg);

void device_vima_print(device_t *dev);
WS_dev_cmd_t *device_vima_stringfy(toklex_t *tl);
void device_vima_commit(WS_dev_t *dev);

int64_t vm_load(vima_t *vm, uint64_t address);
void vm_store(vima_t *vm, uint64_t address, int64_t data);



vima_t *vm_init(uint64_t memory_size);
void vm_step(vima_t *vm);
void vm_setmemory(vima_t *vm, int64_t *memory, uint64_t size);
void vm_print_mem(vima_t *vm, uint64_t start, uint64_t stop);
#endif