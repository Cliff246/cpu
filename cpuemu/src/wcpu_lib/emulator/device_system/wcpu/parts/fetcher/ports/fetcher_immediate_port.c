#include "fetcher_immediate_port.h"
#include <stdlib.h>
#include <assert.h>

fetcher_port_ptr_t fetcher_port_immediate_create(void)
{
	fetcher_port_immediate_t *imm = calloc(1, sizeof(fetcher_port_immediate_t));
	assert(imm != NULL);
	fetcher_port_ptr_t port;
	port.imm = imm;
	return port;
}