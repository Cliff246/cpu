#ifndef __STAGE_ACTOR_HEADER__
#define __STAGE_ACTOR_HEADER__

//
#include "STAGE_costume.h"
#include "TAG_tag.h"

//stage headders
#include "STAGE_role.h"
#include "STAGE_commons.h"
#include "STAGE_prop.h"

//std
#include <stdint.h>

typedef struct STAGE_actor
{
	STAGE_rid_t role_id;
	STAGE_aid_t actor_id;
	char *name;
	TAG_tag_t *tags;
	uint64_t costumes_alloc;
	uint64_t costumes_size;
	STAGE_costume_t **costumes;
	uint64_t props_alloc;
	uint64_t props_size;
	STAGE_prop_t **props;
}STAGE_actor_t;

STAGE_actor_t *STAGE_init_actor(TAG_tag_t *tag, char *name);



#endif
