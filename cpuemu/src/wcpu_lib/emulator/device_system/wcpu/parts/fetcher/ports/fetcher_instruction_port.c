#include "fetcher_instruction_port.h"
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

fetcher_port_ptr_t fetcher_port_instruction_create(void)
{
	fetcher_port_instruction_t *ins = calloc(1, sizeof(fetcher_port_instruction_t));
	assert(ins != NULL);
	fetcher_port_ptr_t port;
	port.ins = ins;

	return port;
}