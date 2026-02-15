#ifndef __SIM_GRAPHMAP_HEADER__
#define __SIM_GRAPHMAP_HEADER__

#include "SIM_device.h"
#include "SIM_commons.h"
#include <stdint.h>

typedef struct RUN_idval
{
	SIM_dtag_t dtag;
	SIM_did_t did;

}RUN_idval_t;

typedef struct RUN_idmap
{
	uint64_t size;
	RUN_idval_t *map;


}RUN_idmap_t;

void RUN_init_idmap(RUN_idmap_t *map, SIM_dtag_t *tags, SIM_did_t *ids, uint64_t size);

SIM_did_t RUN_get_did_idmap(RUN_idmap_t *map, SIM_dtag_t tag);
#endif