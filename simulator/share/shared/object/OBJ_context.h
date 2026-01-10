#ifndef __WS_OBJ_CONTEXT_HEADER__
#define __WS_OBJ_CONTEXT_HEADER__

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "OBJ_handle.h"




typedef struct WS_OBJ_chunk
{
	//target hid
	OBJ_hnd_t thid;
	uint8_t bytes[];
}OBJ_chk_t;

//local context of data traveling between parts
typedef struct WS_OBJ_context
{

	//must be aligned with and / 2
	size_t split;
	size_t size;
	_Alignas(sizeof(void *)) OBJ_chk_t chunks[];

}OBJ_ctx_t;

#endif