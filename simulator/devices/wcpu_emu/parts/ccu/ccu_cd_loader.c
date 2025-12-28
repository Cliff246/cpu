#include "fetcher_port_ptr.h"
#include "fetcher_port_order.h"

#include "ccu_cd_loader.h"
#include "wcpu_part_signal_cd_transfer.h"
#include "wcpu_part.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

/*

void fetcher_port_code_description_create(fetcher_port_t *port)
{
	assert(port);
	fetcher_port_code_descriptor_t *cd = calloc(1, sizeof(fetcher_port_code_descriptor_t));
	assert(cd != NULL && "cd is not null");
	port->port.cd = cd;
	port->produces_mask = FETCHER_PORT_CAPACITY_CODE_DESCRIPTOR;
	port->requires_mask = 0;
}

uint64_t fetcher_port_code_description_issue(fetcher_port_t *port)
{

	assert(port != NULL);
	assert(port->type == WCPU_FETCHER_PORT_CODE_DESCRIPTOR);
	assert(port->port.raw != NULL);
	fetcher_port_code_descriptor_t *cd = port->port.cd;
	uint64_t address = cd->active_context.address + cd->active_context.index;
	assert(address < cd->active_context.address + 6);




	return address;
}

bool fetcher_port_code_description_poll(fetcher_port_t *port, uint64_t address)
{
	assert(port != NULL);
	assert(port->type == WCPU_FETCHER_PORT_CODE_DESCRIPTOR);
	assert(port->port.raw != NULL);
	fetcher_port_code_descriptor_t *cd = port->port.cd;
	//printf("%d %d\n", address, cd->index);
	if(address == cd->active_context.address + cd->active_context.index)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void fetcher_port_code_description_import(fetcher_port_t *port, uint64_t address, int64_t data)
{
	assert(port != NULL);
	assert(port->type == WCPU_FETCHER_PORT_CODE_DESCRIPTOR);
	assert(port->port.raw != NULL);
	fetcher_port_code_descriptor_t *cd = port->port.cd;

	//printf("cd->addr - addr %d %d %d\n", cd->index, address, data);
	cd->active_context.buffer[cd->active_context.index] = data;
	cd->active_context.index++;

}


part_signal_t *fetcher_port_code_description_export(fetcher_port_t *port)
{
	assert(port != NULL);
	assert(port->type == WCPU_FETCHER_PORT_CODE_DESCRIPTOR);
	assert(port->port.raw != NULL);
	fetcher_port_code_descriptor_t *cd = port->port.cd;
	_part_signal_CD_TRANSFER_t *signal = calloc(1, sizeof(_part_signal_CD_TRANSFER_t));
	assert(signal);
	signal->address = cd->active_context.address;
	signal->ct_base = cd->active_context.buffer[0];
	signal->ct_len = cd->active_context.buffer[1];
	signal->ins_base = cd->active_context.buffer[2];
	signal->ins_len = cd->active_context.buffer[3];
	signal->imm_base = cd->active_context.buffer[4];
	signal->imm_len = cd->active_context.buffer[5];

	part_signal_content_ptr_t ptr;
	ptr.CD_TRANSFER = signal;

	part_signal_t *transfer = part_signal_create(PART_SIGNAL_TYPE_CD_TRANSFER, WCPU_PART_FETCHER, WCPU_PART_REGFILE, ptr);
	assert(transfer);
	port->ready = false;
	return transfer;
}


void fetcher_port_code_description_flush(fetcher_port_t *port)
{
	assert(port != NULL);
	assert(port->type == WCPU_FETCHER_PORT_CODE_DESCRIPTOR);
	assert(port->port.raw != NULL);
	fetcher_port_code_descriptor_t *cd = port->port.cd;
	memset(cd->active_context.buffer, 0, sizeof(int64_t) * 6);
	cd->active_context.index = 0;
	cd->active_context.address = 0;

}

bool fetcher_port_code_description_cycle(fetcher_port_t *port)
{
	assert(port != NULL);
	assert(port->type == WCPU_FETCHER_PORT_CODE_DESCRIPTOR);
	assert(port->port.raw != NULL);
	fetcher_port_code_descriptor_t *cd = port->port.cd;

	if(cd->active_context.index >= 6)
	{
		return false;
	}
	return true;

}

void fetcher_port_code_description_order(fetcher_port_t *port, fetcher_port_order_t *order)
{
	assert(port != NULL);
	assert(port->type == WCPU_FETCHER_PORT_CODE_DESCRIPTOR);
	assert(port->port.raw != NULL);
	assert(order);
	fetcher_port_code_descriptor_t *cd = port->port.cd;
	port->state = FETCHER_PORT_ISSUE;
	port->ready = true;
}
//

*/