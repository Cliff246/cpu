#ifndef __WCPU_PART_FETCHER_INSTRUCTION_PORT_HEADER__
#define __WCPU_PART_FETCHER_INSTRUCTION_PORT_HEADER__

#include "fetcher_port_ptr.h"
#include "fetcher_port.h"

#include <stdint.h>


struct wcpu_fetcher_port_instruction
{
	uint64_t buffer[8];
	uint64_t pc_start;
	uint64_t pc_length;
	uint64_t pc;
	uint32_t index;
};

void fetcher_port_instruction_create(fetcher_port_t *port);

#endif