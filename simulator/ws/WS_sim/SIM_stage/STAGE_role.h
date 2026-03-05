#ifndef __STAGE_ROLE_HEADER__
#define __STAGE_ROLE_HEADER__

#include "STAGE_commons.h"

//sim
#include "SIM_commons.h"

#include <stdint.h>

SIM_rid_t STAGE_get_rid(char *role);
char *STAGE_get_str_rid(SIM_rid_t rid);

#endif

