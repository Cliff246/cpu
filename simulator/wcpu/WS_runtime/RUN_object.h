#ifndef __RUN_OBJECT_HEADER__
#define __RUN_OBJECT_HEADER__

#include "RES_pool.h"
#include "RES_resource.h"
#include "OBJ_vtable.h"
#include "SIM_device.h"

typedef struct RUN_object
{
	OBJ_vtable_t vtable;
	RES_pool_t *pool;
	SIM_device_t *device;
}RUN_obj_t;

#endif