#ifndef __WCPU_PART_FETCHER_REQUEST_HEADER__
#define __WCPU_PART_FETCHER_REQUEST_HEADER__

#include "fetcher_port_ptr.h"


#include <stdint.h>
#include <stdbool.h>


typedef struct wcpu_part_fetcher_entry
{
	uint64_t address;
	int64_t data;
	int time;
	bool ready;
}fetcher_entry_t;

void wcpu_part_fetcher_print_entry(fetcher_entry_t entry);

#endif
