#include "fetcher_instruction_port.h"
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "fetcher_port_ptr.h"
#include "fetcher_port.h"

void fetcher_port_instruction_create(fetcher_port_t *port)
{
	fetcher_port_instruction_t *ins = calloc(1, sizeof(fetcher_port_instruction_t));
	assert(ins != NULL);

	port->port.ins = ins;
	port->requires_mask = FETCHER_PORT_CAPACITY_CODE_TABLE;
	port->produces_mask = FETCHER_PORT_CAPACITY_INSTRUCTION;

}

uint64_t fetcher_port_instruction_issue(fetcher_port_t *port)
{

}

bool fetcher_port_instruction_poll(fetcher_port_t *port, uint64_t address)
{

}

void fetcher_port_instruction_import(fetcher_port_t *port, uint64_t address, int64_t data)
{

}
part_signal_t *fetcher_port_instruction_export(fetcher_port_t *port)
{

}

void fetcher_port_instruction_flush(fetcher_port_t *port)
{

}

bool fetcher_port_instruction_cycle(fetcher_port_t *port)
{

}

void fetcher_port_instruction_order(fetcher_port_t *port, fetcher_port_order_ptr_t order)
{

}

void fetcher_port_instruction_order_free(fetcher_port_order_ptr_t order)
{

}