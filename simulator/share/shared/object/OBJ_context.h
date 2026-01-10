#ifndef __WS_OBJ_CONTEXT_HEADER__
#define __WS_OBJ_CONTEXT_HEADER__

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>




typedef struct WS_OBJ_context
{
	void *ctx_hnd;

	size_t size;
	uint8_t *mem;

}OBJ_ctx_t;

#endif