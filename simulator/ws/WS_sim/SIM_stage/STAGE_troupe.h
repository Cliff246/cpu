#ifndef __STAGE_TROUPE_HEADER__
#define __STAGE_TROUPE_HEADER__

#include "STAGE_role.h"
#include "hashmap.h"
#include <stdint.h>

typedef struct STAGE_troupe
{
	p_hashtable_t roles;
}STAGE_troupe_t;

STAGE_troupe_t *STAGE_init_troupe(STAGE_role_t *roles, uint64_t size);

#endif