#ifndef __WS_OBJ_LOCALS_HEADER__
#define __WS_OBJ_LOCALS_HEADER__

#include <stdint.h>
#include <stdlib.h>
#include <stdalign.h>

//held by the update function only.
typedef struct WS_OBJ_locals
{
	size_t size;
	_Alignas(8) uint8_t bytes[];
}OBJ_locals_t;

#endif