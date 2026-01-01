#ifndef __VIRTUAL_MACHINE_HEADER__
#define __VIRTUAL_MACHINE_HEADER__

#include <stdint.h>

typedef struct virtual_machine vima_t;

#include "vm_bus.h"
#include "vm_regs.h"
#include "vm_op.h"
#include "vm_txn.h"

#include "device.h"
#include "token.h"
#include "device_message.h"
#include "device_command.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#define STAGES_COUNT 5

typedef struct virtual_machine
{
	vm_bus_t bus;

	vm_bookmark_t bookmarks[BOOKMARKS_MAX];

	vm_regfile_t regs;
	bool wait_until_inst_end;
	bool load_cd;
	int cd_count;
	int cd_base;
	vm_bus_port_id_t port;
	vm_bus_hnd_t hnd;
	bool new_inst;
	vm_txn_t *txn;
}vima_t;



void *device_vima_init(device_t *device);

void device_vima_update(device_t *dev);
bool device_vima_read(WS_dev_t *dev, WS_dev_msg_t *msg);
bool device_vima_send(WS_dev_t *dev, WS_dev_msg_t **msg);

void device_vima_print(device_t *dev);
void device_vima_commit(WS_dev_t *dev);



#endif
