#include "device_list.h"

#include <stdlib.h>
#include <assert.h>
#include "device_commons.h"
#include "device_command_impl.h"



/*




WS_dev_t *WS_device_init(MOD_description_t *desc, device_command_t *cmd)
{
	static int devid = 0;
	WS_dev_t *device = calloc(1, sizeof(WS_dev_t));

	device->ptr = desc->vtable->init(device);

	WS_device_cmd(device, desc ,cmd);



	return device;
}






bool WS_get_device_changed(WS_dev_t *device)
{
	assert(device != NULL && "cannot update null device");
	//return device->flags[DEVICE_FLAG_TYPE_INTERNAL_CHANGED];
}

//only sets to high, device manager should clear
void WS_set_device_changed(WS_dev_t *device)
{
	assert(device != NULL && "cannot update null device");
	//device->flags[DEVICE_FLAG_TYPE_INTERNAL_CHANGED] = true;
	return;
}


*/