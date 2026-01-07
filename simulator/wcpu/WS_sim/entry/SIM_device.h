#ifndef __WS_SIM_OBJECT_HEADER__
#define __WS_SIM_OBJECT_HEADER__

#include <stdint.h>
#include <stdbool.h>
#include "device.h"
#include "SIM_port.h"
#include "SIM_tag.h"



typedef struct WS_SIM_device
{
	char *name;
	WS_dev_t *deivce;
	SIM_handle_t handle;



}SIM_device_t;


#endif