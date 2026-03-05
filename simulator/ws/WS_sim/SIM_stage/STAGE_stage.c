#include "STAGE_stage.h"
#include "SIM_commons.h"
#include "STAGE_actor.h"
#include "STAGE_role.h"
#include "commons.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>

#define STAGE_ACTORS_DEFAULT_ALLOC 10


STAGE_stage_t *STAGE_init_stage()
{
	STAGE_stage_t *stage = calloc(1, sizeof(STAGE_stage_t));
	assert(stage);

	stage->actors_alloc = STAGE_ACTORS_DEFAULT_ALLOC;
	stage->actors_size = 0;


	stage->actors = calloc(stage->actors_alloc, sizeof(STAGE_actor_t *));
	assert(stage->actors);
	stage->locked = false;


	return stage;
}



void STAGE_fill_stage(STAGE_stage_t *stage, MANFST_manifest_t *manifest)
{


	stage->locked = false;

	uint64_t add_size = manifest->scopes_size;

	uint64_t current_size = stage->actors_size;
	uint64_t new_minimum_size = add_size + current_size;

	if(new_minimum_size > stage->actors_alloc)
	{
		uint64_t diff = new_minimum_size - stage->actors_alloc;
		stage->actors_alloc += diff * 2;

		stage->actors = realloc_safe(stage->actors, stage->actors_alloc, sizeof(STAGE_actor_t *));
	}

	for(uint64_t i = 0; stage->actors_size < new_minimum_size; ++i, ++stage->actors_size)
	{
		STAGE_actor_t *actor = STAGE_alloc_actor(stage->actors_size);

		SIM_rid_t role_id = STAGE_get_rid(manifest->scopes[i].code);

		STAGE_init_actor(actor, manifest->scopes[i].scope, role_id);
		stage->actors[stage->actors_size] = actor;

	}
}

void STAGE_lock_stage(STAGE_stage_t *stage)
{
	stage->locked = true;
}

void STAGE_print_stage(STAGE_stage_t *stage)
{
	for(uint64_t i = 0; i < stage->actors_size; ++i)
	{
		STAGE_print_actor(stage->actors[i]);
	}
}