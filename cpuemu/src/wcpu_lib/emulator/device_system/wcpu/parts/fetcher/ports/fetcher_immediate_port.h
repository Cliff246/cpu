#ifndef __WCPU_PART_FETCHER_IMMEDIATE_PORT_HEADER__
#define __WCPU_PART_FETCHER_IMMEDIATE_PORT_HEADER__

#include "fetcher_port_ptr.h"
#include "fetcher_port.h"
#include <stdint.h>

struct wcpu_fetcher_port_immediate
{
	int64_t buffer[8];
	uint64_t ipc_start;
	uint64_t ipc_length;
	uint64_t ipc;
	uint32_t index;

};

void fetcher_port_immediate_create(fetcher_port_t *port);



#endif