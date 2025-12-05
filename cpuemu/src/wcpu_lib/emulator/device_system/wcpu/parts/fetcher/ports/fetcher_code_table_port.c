#include "fetcher_code_table_port.h"
#include <stdlib.h>
#include <assert.h>

fetcher_port_ptr_t fetcher_port_code_table_create(void)
{
	fetcher_port_code_table_t *ct = calloc(1, sizeof(fetcher_port_code_table_t));
	assert(ct != NULL);
	fetcher_port_ptr_t port;
	port.ct = ct;
	return port;
}
