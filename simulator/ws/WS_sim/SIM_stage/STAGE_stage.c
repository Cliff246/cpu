#include "STAGE_stage.h"
#include "STAGE_actor.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

STAGE_stage_t *STAGE_init_stage(STAGE_troupe_t *troupe)
{
	STAGE_stage_t *stage = calloc(1, sizeof(STAGE_stage_t));
	stage->troupe = troupe;
	return stage;
}



void STAGE_fill_stage(STAGE_stage_t *stage, MANFST_manifest_t *manifest)
{
	stage->actors = calloc(manifest->scopes_size, sizeof(STAGE_actor_t *));
	stage->actors_size = manifest->scopes_size;

	for(uint64_t i = 0; i < stage->actors_size; ++i)
	{
		stage->actors[i] = STAGE_init_actor(manifest->scopes[i].scope, manifest->scopes[i].code);

	}
}

void STAGE_print_stage(STAGE_stage_t *stage)
{
	for(uint64_t i = 0; i < stage->actors_size; ++i)
	{
		STAGE_print_actor(stage->actors[i]);
	}
}