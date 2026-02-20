#ifndef __OBJ_RESOURCE_HEADER__
#define __OBJ_RESOURCE_HEADER__

#include "OBJ_commons.h"
#include <stdint.h>

typedef struct OBJ_resource
{
	OBJ_resrc_hnd_t hnd;
	OBJ_resrc_type_t type;
	uint64_t used;
	uint64_t size;
	_Alignas(8) uint8_t *bytes;
}OBJ_resource_t;

#endif