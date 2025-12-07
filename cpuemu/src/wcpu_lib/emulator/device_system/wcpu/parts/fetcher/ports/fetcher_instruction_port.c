#include "fetcher_instruction_port.h"
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "fetcher_instruction_port.h"
#include "fetcher_port.h"

void fetcher_port_instruction_create(fetcher_port_t *port)
{
	fetcher_port_instruction_t *ins = calloc(1, sizeof(fetcher_port_instruction_t));
	assert(ins != NULL);

	port->port.ins = ins;
	port->requires_mask = FETCHER_PORT_CAPACITY_CODE_TABLE;
	port->produces_mask = FETCHER_PORT_CAPACITY_INSTRUCTION;

}