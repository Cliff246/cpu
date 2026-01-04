#ifndef __SIMULATOR_DEVICE_HEADER__
#define __SIMULATOR_DEVICE_HEADER__


#include "SIM_handle.h"
typedef struct WS_SIM_device
{
	SIM_handle_t *hnd;
	int count;

}SIM_device_t;

#endif