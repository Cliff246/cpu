#include "STAGE_troupe.h"
#include "STAGE_role.h"
#include <stdlib.h>



STAGE_troupe_t *STAGE_init_troupe(STAGE_role_t *roles, uint64_t size)
{
	STAGE_troupe_t *troupe = calloc(1, sizeof(STAGE_troupe_t));


	return troupe;
}