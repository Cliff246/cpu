#include "fetcher_port_ptr.h"
#include "fetcher_code_descriptor_port.h"
#include <stdlib.h>
#include <assert.h>

void fetcher_port_code_description_create(fetcher_port_t *port)
{
	fetcher_port_code_descriptor_t *cd = calloc(1, sizeof(fetcher_port_code_descriptor_t));
	assert(cd != NULL && "cd is not null");
	port->port.cd = cd;
	port->produces_mask = FETCHER_PORT_CAPACITY_CODE_DESCRIPTOR;
	port->requires_mask = 0;
}