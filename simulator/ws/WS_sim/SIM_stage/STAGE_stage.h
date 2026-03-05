#ifndef __STAGE_STAGE_HEADER__
#define __STAGE_STAGE_HEADER__

#include "MANFST_manifest.h"
#include "STAGE_actor.h"
#include "hashmap.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct STAGE_stage
{

	uint64_t actors_size;
	uint64_t actors_alloc;
	STAGE_actor_t **actors;

	bool locked;

}STAGE_stage_t;


STAGE_stage_t *STAGE_init_stage();



void STAGE_fill_stage(STAGE_stage_t *stage, MANFST_manifest_t *manifest);
void STAGE_lock_stage(STAGE_stage_t *stage);



void STAGE_print_stage(STAGE_stage_t *stage);

#endif
