#ifndef __WCPU_PART_SIGNAL_CORE_IO_HEADER__
#define __WCPU_PART_SIGNAL_CORE_IO_HEADER__

#include "wcpu_part_signal_ptr.h"

#include <stdint.h>

struct wcpu_part_signal_CORE_INPUT
{
	int64_t addr;
};

struct wcpu_part_signal_CORE_OUTPUT
{
	int64_t addr;
	int64_t value;

};

struct wcpu_part_signal_CORE_MEM_RESPONSE
{
	int64_t address;
	int64_t load_value;
};

#endif