#ifndef __OBJ_LOCAL_HEADER__
#define __OBJ_LOCAL_HEADER__

#include <stdint.h>
#include <stdalign.h>

typedef struct OBJ_local
{
	uint64_t bytes;
	uint8_t *local;
}OBJ_local_t;

#endif