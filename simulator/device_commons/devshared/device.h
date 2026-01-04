#ifndef __DEVICE_HEADER__
#define __DEVICE_HEADER__

#include "device_list.h"

#include "device_message.h"
#include "device_mailbox.h"

//#include "emulator_configure.h"


#include <stdint.h>
#include <stdlib.h>

typedef struct device_description WS_dev_desc_t;
//set up device

#define WS_SIMULATOR_DEVICE_SIZE

typedef struct WS_simulator_device
{
	void *ptr;
}device_t, WS_dev_t;






#endif