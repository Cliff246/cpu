#ifndef __WCPU_PART_FETCHER_CODE_DESCRIPTOR_PORT_CONTEXT_HEADER__
#define __WCPU_PART_FETCHER_CODE_DESCRIPTOR_PORT_CONTEXT_HEADER__

#define WCPU_CODE_DESCRIPTOR_SIZE 6


#include "wcpu_part_signal.h"

#include <stdint.h>
#include <stdbool.h>

//stupid addition
typedef struct wcpu_fetcher_port_code_descriptor_context
{
	int64_t buffer[WCPU_CODE_DESCRIPTOR_SIZE];
	uint32_t index;
	uint64_t address;

}fetcher_port_cd_ctx_t;

#endif