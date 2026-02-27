#include "STAGE_actor.h"
#include "TAG_tag.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

STAGE_actor_t *STAGE_init_actor(TAG_tag_t *tag, char *name)
{
	STAGE_actor_t *actor = calloc(1, sizeof(STAGE_actor_t));
	assert(name);
	actor->name = strdup(name);

	TAG_tag_t *actor_tags = TAG_copy(tag);

	actor->tags = actor_tags;

	return actor;
}