#ifndef __WS_OBJ_OUTPUT_HEADER__
#define __WS_OBJ_OUTPUT_HEADER__

#include "flags.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdalign.h>

//todo this is temp
typedef struct WS_OBJ_message_output
{
	uint64_t address;
	uint64_t data[CACHE_LINE_SIZE];
}OBJ_msgout_t;




#endif