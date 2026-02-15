#ifndef __RES_POOL_HEADER__
#define __RES_POOL_HEADER__

#include "RES_resource.h"

#include <stdint.h>

typedef struct RES_pool
{
	uint64_t size;
	RES_resource_t **resources;

}RES_pool_t;

#endif