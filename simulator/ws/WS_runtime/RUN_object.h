#ifndef __RUN_OBJECT_HEADER__
#define __RUN_OBJECT_HEADER__

#include "RES_pool.h"
#include "RES_resource.h"
#include "SIM_device.h"
#include "RES_interface.h"

typedef struct RUN_object
{
	RES_itrfc_t *interface;
	SIM_device_t *device;
}RUN_obj_t;

#endif