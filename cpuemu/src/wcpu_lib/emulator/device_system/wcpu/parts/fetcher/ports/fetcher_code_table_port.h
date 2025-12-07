#ifndef __WCPU_PART_FETCHER_CODE_TABLE_PORT_HEADER__
#define __WCPU_PART_FETCHER_CODE_TABLE_PORT_HEADER__


#include "fetcher_port_ptr.h"
#include "fetcher_port.h"
#include <stdint.h>


struct wcpu_fetcher_port_code_table
{
	uint64_t buffer[64];
	uint64_t ct_start;
	uint64_t ct_length;
	uint32_t index;

};

void fetcher_port_code_table_create(fetcher_port_t *port);



#endif