#ifndef __RUN_OBJECT_HEADER__
#define __RUN_OBJECT_HEADER__

#include "RES_pool.h"
#include "RES_resource.h"
#include "SCENE_device.h"
#include "RES_interface.h"

typedef struct RUN_object
{
	RES_itrfc_t *interface;
	SCENE_device_t *device;
}RUN_obj_t;

#endif
