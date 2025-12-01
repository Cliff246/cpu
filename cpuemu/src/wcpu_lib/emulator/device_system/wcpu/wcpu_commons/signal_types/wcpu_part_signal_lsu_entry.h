#ifndef __WCPU_PART_SIGNAL_LSU_ENTRY_HEADER__
#define __WCPU_PART_SIGNAL_LSU_ENTRY_HEADER__

#include "wcpu_part_signal_ptr.h"
#include <stdint.h>
#include <stdbool.h>

//has to be exactly this name
struct wcpu_part_signal_LSU
{
	bool loadstore;
	int64_t address;
	int64_t value;

};

#endif