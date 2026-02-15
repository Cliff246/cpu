#ifndef __RES_INTERFACE_HEADER__
#define __RES_INTERFACE_HEADER__

#include "RES_alias.h"
#include <stdint.h>

typedef struct RES_interface
{
	uint64_t count;
	RES_alias_t *aliases;
}RES_itrfc_t;

#endif