#include "fetcher_instruction_port.h"

#include "fetcher_port_ptr.h"
#include "fetcher_port.h"

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

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
	assert(port);
	assert(port->type == WCPU_FETCHER_PORT_INSTRUCTION);
	assert(port->port.raw != NULL);
	fetcher_port_instruction_t *ins = port->port.ins;


	assert(ins->pc_length < (ins->base + ins->index + ins->pc_start));

	assert(ins->index < 8);
	return ins->pc_start + ins->index;
}

bool fetcher_port_instruction_poll(fetcher_port_t *port, uint64_t address)
{
	assert(port);
	assert(port->type == WCPU_FETCHER_PORT_INSTRUCTION);
	assert(port->port.raw != NULL);
	fetcher_port_instruction_t *ins = port->port.ins;


	if(address == (ins->pc_start + ins->base + ins->index))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void fetcher_port_instruction_import(fetcher_port_t *port, uint64_t address, int64_t data)
{
	assert(port);
	assert(port->type == WCPU_FETCHER_PORT_INSTRUCTION);
	assert(port->port.raw != NULL);
	fetcher_port_instruction_t *ins = port->port.ins;

	assert(address == (ins->pc_start + ins->base + ins->index));

	ins->buffer[ins->index] = data;

}
part_signal_t *fetcher_port_instruction_export(fetcher_port_t *port)
{
	assert(port);
	assert(port->type == WCPU_FETCHER_PORT_INSTRUCTION);
	assert(port->port.raw != NULL);
	fetcher_port_instruction_t *ins = port->port.ins;


	return NULL;
}

void fetcher_port_instruction_flush(fetcher_port_t *port)
{
	assert(port);
	assert(port->type == WCPU_FETCHER_PORT_INSTRUCTION);
	assert(port->port.raw != NULL);
	fetcher_port_instruction_t *ins = port->port.ins;


	ins->base = 0;
	//TODO make that a define
	memset(ins->buffer, 0, sizeof(int64_t) * 8);
	ins->index = 0;
	
}

bool fetcher_port_instruction_cycle(fetcher_port_t *port)
{
	assert(port);
	assert(port->type == WCPU_FETCHER_PORT_INSTRUCTION);
	assert(port->port.raw != NULL);
	fetcher_port_instruction_t *ins = port->port.ins;
}

void fetcher_port_instruction_order(fetcher_port_t *port, fetcher_port_order_ptr_t order)
{
	assert(port);
	assert(port->type == WCPU_FETCHER_PORT_INSTRUCTION);
	assert(port->port.raw != NULL);
	fetcher_port_instruction_t *ins = port->port.ins;
}

void fetcher_port_instruction_order_free(fetcher_port_order_ptr_t order)
{
	free(order.ins);
}