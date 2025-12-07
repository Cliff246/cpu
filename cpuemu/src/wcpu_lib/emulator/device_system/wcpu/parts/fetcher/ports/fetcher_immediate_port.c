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