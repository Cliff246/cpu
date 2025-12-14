#ifndef __DEVICE_COMMAND_HEADER__
#define __DEVICE_COMMAND_HEADER__


#include "device_list.h"
#include "device_description.h"

#include <stdbool.h>
typedef struct device_description WS_dev_desc_t;

typedef struct wcpu_simulator_device_command
{
	//used to expire this device ptr
	bool used;
	WS_dev_desc_t *type;
	void *setting;
}device_command_t, WS_dev_cmd_t;









#endif