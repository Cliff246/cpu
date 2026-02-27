#ifndef __RUN_SCHEDULE_HEADER__
#define __RUN_SCHEDULE_HEADER__


#include "RUN_operation.h"

#include <stdint.h>


typedef struct RUN_schedule
{
	uint64_t size;
	RUN_op_t *operations;
}RUN_schedule_t;

#endif