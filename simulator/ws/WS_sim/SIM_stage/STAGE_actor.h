#ifndef __STAGE_ACTOR_HEADER__
#define __STAGE_ACTOR_HEADER__

//
#include "TAG_tag.h"

//stage headders
#include "STAGE_role.h"
#include "STAGE_commons.h"

//
#include "SIM_commons.h"
//std
#include <stdint.h>

typedef struct STAGE_actor
{
	SIM_rid_t role_id;
	SIM_aid_t actor_id;
	TAG_tag_t *tags;

}STAGE_actor_t;

void STAGE_init_actor_costumes(STAGE_actor_t *actor);

STAGE_actor_t *STAGE_alloc_actor(SIM_aid_t aid);

void STAGE_init_actor(STAGE_actor_t *actor, TAG_tag_t *tag, SIM_rid_t rid);

void STAGE_print_actor(STAGE_actor_t *actor);


#endif
