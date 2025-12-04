#ifndef __WCPU_PART_SIGNAL_REGISTER_TRANSFER_HEADER__
#define __WCPU_PART_SIGNAL_REGISTER_TRANSFER_HEADER__

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "wcpu_part_signal_ptr.h"


#define WCPU_REGISTER_CODE_SIZE 5

struct wcpu_part_signal_REGISTER_TRANSFER
{
	//pull
	bool loadstore;
	int64_t address;
	int64_t value;
};

#endif