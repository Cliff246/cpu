#include "fetcher_port_ptr.h"
#include "fetcher_port_order.h"

#include "fetcher_code_descriptor_port.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

void fetcher_port_code_description_create(fetcher_port_t *port)
{
	fetcher_port_code_descriptor_t *cd = calloc(1, sizeof(fetcher_port_code_descriptor_t));
	assert(cd != NULL && "cd is not null");
	port->port.cd = cd;
	port->produces_mask = FETCHER_PORT_CAPACITY_CODE_TABLE;
	port->requires_mask = 0;
}

uint64_t fetcher_port_code_description_issue(fetcher_port_t *port)
{

	assert(port != NULL);
	assert(port->type == WCPU_FETCHER_PORT_CODE_DESCRIPTOR);
	assert(port->port.raw != NULL);
	fetcher_port_code_descriptor_t *cd = port->port.cd;

	uint64_t address = cd->address + cd->index;




	return address;
}

bool fetcher_port_code_description_poll(fetcher_port_t *port, uint64_t address)
{
	assert(port != NULL);
	assert(port->type == WCPU_FETCHER_PORT_CODE_DESCRIPTOR);
	assert(port->port.raw != NULL);
	fetcher_port_code_descriptor_t *cd = port->port.cd;
	printf("%d %d\n", address, cd->index);
	if(address == cd->address + cd->index)
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

	printf("cd->addr - addr %d %d %d\n", cd->index, address, data);
	cd->buffer[cd->index] = data;
	cd->index++;

}


part_signal_t *fetcher_port_code_description_export(fetcher_port_t *port)
{
	assert(port != NULL);
	assert(port->type == WCPU_FETCHER_PORT_CODE_DESCRIPTOR);
	assert(port->port.raw != NULL);
	fetcher_port_code_descriptor_t *cd = port->port.cd;

	for(int i = 0; i < 6; ++i)
	{
		printf("%d\n", cd->buffer[i]);
	}

	return NULL;
}


void fetcher_port_code_description_flush(fetcher_port_t *port)
{
	assert(port != NULL);
	assert(port->type == WCPU_FETCHER_PORT_CODE_DESCRIPTOR);
	assert(port->port.raw != NULL);
	fetcher_port_code_descriptor_t *cd = port->port.cd;

	memset(cd->buffer, 0, sizeof(int64_t));
	cd->index = 0;
	cd->address = 0;

}

bool fetcher_port_code_description_cycle(fetcher_port_t *port)
{
	assert(port != NULL);
	assert(port->type == WCPU_FETCHER_PORT_CODE_DESCRIPTOR);
	assert(port->port.raw != NULL);
	fetcher_port_code_descriptor_t *cd = port->port.cd;

	if(cd->index >= 6)
	{
		return false;
	}
	return true;

}

void fetcher_port_code_description_order(fetcher_port_t *port, fetcher_port_order_ptr_t order)
{
	assert(port != NULL);
	assert(port->type == WCPU_FETCHER_PORT_CODE_DESCRIPTOR);
	assert(port->port.raw != NULL);


	port->state = FETCHER_PORT_ISSUE;
}
//
void fetcher_port_code_description_order_free(fetcher_port_order_ptr_t order)
{
	free(order.cd);
}

