#ifndef __WS_OBJ_CONTEXT_HEADER__
#define __WS_OBJ_CONTEXT_HEADER__

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdalign.h>
#include "OBJ_handle.h"






//local context of data traveling between parts
typedef struct WS_OBJ_context
{

	//must be aligned with and / 2
	const size_t size;
	_Aligned(8) uint8_t bytes[];

}OBJ_ctx_t;

#endif