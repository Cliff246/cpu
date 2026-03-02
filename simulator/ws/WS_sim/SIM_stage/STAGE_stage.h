#ifndef __STAGE_STAGE_HEADER__
#define __STAGE_STAGE_HEADER__

#include "MANFST_manifest.h"
#include "STAGE_actor.h"
#include "STAGE_troupe.h"

#include <stdint.h>

typedef struct STAGE_stage
{
	STAGE_troupe_t *troupe;
	uint64_t actors_size;
	STAGE_actor_t **actors;
}STAGE_stage_t;

STAGE_stage_t *STAGE_init_stage(STAGE_troupe_t *troupe);
void STAGE_fill_stage(STAGE_stage_t *stage, MANFST_manifest_t *manifest);
void STAGE_print_stage(STAGE_stage_t *stage);

#endif
