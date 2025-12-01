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
	bool pull_reg_a;
	char regcode_a[WCPU_REGISTER_CODE_SIZE];
	bool put_reg_b;
	char regcode_b[WCPU_REGISTER_CODE_SIZE];

	int64_t content;
};

#endif