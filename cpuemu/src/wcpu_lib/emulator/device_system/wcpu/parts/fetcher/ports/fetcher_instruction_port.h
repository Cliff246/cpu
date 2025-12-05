#ifndef __WCPU_PART_FETCHER_INSTRUCTION_PORT_HEADER__
#define __WCPU_PART_FETCHER_INSTRUCTION_PORT_HEADER__

#include "fetcher_port_ptr.h"

#include <stdint.h>


struct wcpu_fetcher_port_instruction
{
	uint64_t buffer[8];
	uint64_t pc_start;
	uint64_t pc_length;
	uint64_t pc;
	uint32_t index;
};

fetcher_port_ptr_t fetcher_port_instruction_create(void);

#endif