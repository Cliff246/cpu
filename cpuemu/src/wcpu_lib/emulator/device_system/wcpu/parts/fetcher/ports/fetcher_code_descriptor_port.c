#include "fetcher_port_ptr.h"
#include "fetcher_code_descriptor_port.h"
#include <stdlib.h>
#include <assert.h>

fetcher_port_ptr_t fetcher_port_code_description_create(void)
{
	fetcher_port_code_descriptor_t *cd = calloc(1, sizeof(fetcher_port_code_descriptor_t));
	assert(cd != NULL && "cd is not null");
	fetcher_port_ptr_t port;
	port.cd = cd;
	return port;
}