#ifndef __WS_OBJ_INPUT_HEADER__
#define __WS_OBJ_INPUT_HEADER__

#include "flags.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdalign.h>


//todo this is temp
typedef struct WS_OBJ_message_input
{

	uint64_t data[CACHE_LINE_SIZE];
	uint8_t len;
}OBJ_msgin_t;



#endif