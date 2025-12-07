#include "fetcher_immediate_port.h"
#include "fetcher_port.h"
#include <stdlib.h>
#include <assert.h>


void fetcher_port_immediate_create(fetcher_port_t *port)
{
	fetcher_port_immediate_t *imm = calloc(1, sizeof(fetcher_port_immediate_t));
	assert(imm != NULL);

	port->port.imm = imm;
	port->requires_mask = FETCHER_PORT_CAPACITY_CODE_TABLE;
	port->produces_mask = FETCHER_PORT_CAPACITY_IMMEDIATE;

}



uint64_t fetcher_port_immediate_issue(fetcher_port_t *port)
{

}

bool fetcher_port_immediate_poll(fetcher_port_t *port, uint64_t address)
{

}

void fetcher_port_immediate_import(fetcher_port_t *port, uint64_t address, int64_t data)
{

}


part_signal_t *fetcher_port_immediate_export(fetcher_port_t *port)
{

}


void fetcher_port_immediate_flush(fetcher_port_t *port)
{

}

bool fetcher_port_immediate_cycle(fetcher_port_t *port)
{

}

void fetcher_port_immediate_order(fetcher_port_t *port, fetcher_port_order_ptr_t order)
{

}



void fetcher_port_immediate_order_free(fetcher_port_order_ptr_t order)
{

}