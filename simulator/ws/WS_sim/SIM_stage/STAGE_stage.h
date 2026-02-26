#ifndef __STAGE_STAGE_HEADER__
#define __STAGE_STAGE_HEADER__

#include "STAGE_actor.h"
#include <stdint.h>

typedef struct STAGE_stage
{
	uint64_t actors_size;
	STAGE_actor_t **actors;
}STAGE_stage_t;

STAGE_stage_t *STAGE_init_stage(void);
#endif
