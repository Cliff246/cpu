#include "fetcher_code_table_port.h"
#include <wcpu_part_signal.h>

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

void fetcher_port_code_table_create(fetcher_port_t *port)
{
	fetcher_port_code_table_t *ct = calloc(1, sizeof(fetcher_port_code_table_t));
	assert(ct != NULL);
	port->port.ct = ct;

	port->requires_mask = FETCHER_PORT_CAPACITY_CODE_DESCRIPTOR;
	port->produces_mask = FETCHER_PORT_CAPACITY_CODE_TABLE;

}

uint64_t fetcher_port_code_table_issue(fetcher_port_t *port)
{

}

bool fetcher_port_code_table_poll(fetcher_port_t *port, uint64_t address)
{

}


void fetcher_port_code_table_import(fetcher_port_t *port, uint64_t address, int64_t data)
{

}

part_signal_t *fetcher_port_code_table_export(fetcher_port_t *port)
{

}


void fetcher_port_code_table_flush(fetcher_port_t *port)
{

}

bool fetcher_port_code_table_cycle(fetcher_port_t *port)
{

}

void fetcher_port_code_table_order(fetcher_port_t *port, fetcher_port_order_ptr_t order)
{

}

void fetcher_port_code_table_order_free(fetcher_port_order_ptr_t order)
{

}