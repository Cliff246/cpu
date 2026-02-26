#ifndef __STAGE_ACTOR_HEADER__
#define __STAGE_ACTOR_HEADER__

//
#include "TAG_tag.h"

//stage headders
#include "STAGE_role.h"
#include "STAGE_commons.h"
#include "STAGE_prop.h"

//std 
#include <stdint.h>

typedef struct STAGE_actor
{
	STAGE_aid_t id;

	STAGE_role_t role;
	uint64_t props_alloc;
	uint64_t props_size;
	STAGE_prop_t **props;

}STAGE_actor_t;

#endif
