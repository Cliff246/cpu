#ifndef __WS_OBJ_OBJECT_HEADER__
#define __WS_OBJ_OBJECT_HEADER__

#include "OBJ_global.h"
#include "OBJ_commons.h"
#include <stdint.h>
#include <stdlib.h>


typedef struct WS_OBJ_object
{
	void *monolith;
	//object id
	int oid;
	//simulator id
	int sid;
	OBJ_global_t globals;
	OBJ_commons_t commons;
}OBJ_object_t;

#endif