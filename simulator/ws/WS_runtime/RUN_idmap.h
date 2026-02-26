#ifndef __SIM_GRAPHMAP_HEADER__
#define __SIM_GRAPHMAP_HEADER__

#include "SCENE_device.h"
#include "SIM_commons.h"
#include <stdint.h>

typedef struct RUN_idval
{
	SIM_dkey_t dtag;
	SIM_did_t did;

}RUN_idval_t;

typedef struct RUN_idmap
{
	uint64_t size;
	uint64_t allocd;
	RUN_idval_t *map;


}RUN_idmap_t;

void RUN_alloc_idmap(RUN_idmap_t *map, uint64_t size);
void RUN_build_idmap(RUN_idmap_t *map, SIM_dkey_t *keys, SIM_did_t *ids, uint64_t size);



SIM_did_t RUN_get_did_idmap(RUN_idmap_t *map, SIM_dkey_t key);
#endif
