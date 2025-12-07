#include "fetcher_code_table_port.h"
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
