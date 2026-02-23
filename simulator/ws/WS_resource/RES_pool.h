#ifndef __RES_POOL_HEADER__
#define __RES_POOL_HEADER__

#include "RES_resource.h"

#include <stdint.h>

typedef struct RES_pool
{
	uint64_t pool_size;
	RES_resrc_t **pool;

	uint64_t id_map_size;
	uint64_t *id_map;
}RES_pool_t;

#endif