#ifndef __WCPU_PART_SIGNAL_FETCHER_COMMAND_HEADER__
#define __WCPU_PART_SIGNAL_FETCHER_COMMAND_HEADER__

#include "wcpu_part_signal_ptr.h"

#include <stdint.h>
#include <stdbool.h>

#include "fetcher.h"



struct wcpu_part_signal_FETCHER_COMMAND
{
	bool code_desc_swap;

	int64_t address;

	bool read;

	int64_t pull;
};



#endif