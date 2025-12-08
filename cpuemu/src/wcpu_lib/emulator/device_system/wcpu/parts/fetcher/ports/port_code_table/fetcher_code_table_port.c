#include "fetcher_code_table_port.h"
#include "wcpu_part_signal.h"

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void fetcher_port_code_table_create(fetcher_port_t *port)
{
	assert(port);

	fetcher_port_code_table_t *ct = calloc(1, sizeof(fetcher_port_code_table_t));
	assert(ct != NULL);
	port->port.ct = ct;

	port->requires_mask = FETCHER_PORT_CAPACITY_CODE_DESCRIPTOR;
	port->produces_mask = FETCHER_PORT_CAPACITY_CODE_TABLE;

}

uint64_t fetcher_port_code_table_issue(fetcher_port_t *port)
{
	assert(port);
	assert(port->type == WCPU_FETCHER_PORT_CODE_TABLE);
	assert(port->port.raw != NULL);
	fetcher_port_code_table_t *ct = port->port.ct;



	uint64_t address = ct->ct_start + ct->index;
	return address;
}

bool fetcher_port_code_table_poll(fetcher_port_t *port, uint64_t address)
{
	assert(port);
	assert(port->type == WCPU_FETCHER_PORT_CODE_TABLE);
	assert(port->port.raw != NULL);
	fetcher_port_code_table_t *ct = port->port.ct;

	if(ct->ct_start + ct->index == address)
	{
		return true;
	}
	else
	{
		return false;
	}

}


void fetcher_port_code_table_import(fetcher_port_t *port, uint64_t address, int64_t data)
{
	assert(port);
	assert(port->type == WCPU_FETCHER_PORT_CODE_TABLE);
	assert(port->port.raw != NULL);
	fetcher_port_code_table_t *ct = port->port.ct;
	assert(address > (ct->ct_start));
	assert(address < (ct->ct_length + ct->ct_start));
	ct->buffer[ct->index] = data;
	ct->index++;
}

part_signal_t *fetcher_port_code_table_export(fetcher_port_t *port)
{
	assert(port);
	assert(port->type == WCPU_FETCHER_PORT_CODE_TABLE);
	assert(port->port.raw != NULL);
	fetcher_port_code_table_t *ct = port->port.ct;

	return NULL;
}


void fetcher_port_code_table_flush(fetcher_port_t *port)
{
	assert(port);
	assert(port->type == WCPU_FETCHER_PORT_CODE_TABLE);
	assert(port->port.raw != NULL);
	fetcher_port_code_table_t *ct = port->port.ct;
	ct->ct_length = 0;
	ct->ct_start = 0;
	ct->index = 0;
	memset(ct->buffer, 0, sizeof(uint64_t) * 64);
}

bool fetcher_port_code_table_cycle(fetcher_port_t *port)
{
	assert(port);
	assert(port->type == WCPU_FETCHER_PORT_CODE_TABLE);
	assert(port->port.raw != NULL);
	fetcher_port_code_table_t *ct = port->port.ct;

	if(ct->index >= ct->ct_length)
		return false;
	return true;
}

void fetcher_port_code_table_order(fetcher_port_t *port, fetcher_port_order_t *order)
{
	assert(port);
	assert(port->type == WCPU_FETCHER_PORT_CODE_TABLE);
	assert(port->port.raw != NULL);
	fetcher_port_code_table_t *ct = port->port.ct;

	fetcher_port_order_CODE_TABLE_t ct_order = order->order.CODE_TABLE;
	//this will fuck me i can virtually gurantee it.

	assert(ct->ct_length < 64);
	ct->ct_start = ct_order.ct_base;
	ct->ct_length = ct_order.ct_len;
}
