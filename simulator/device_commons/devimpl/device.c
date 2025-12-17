#include "device_list.h"

#include <stdlib.h>
#include <assert.h>
#include "device.h"
#include "device_commons.h"
#include "device_command_impl.h"





size_t WS_get_device_address_start(WS_dev_t *device)
{
	return device->address_range_start;
}
size_t WS_get_device_address_length(WS_dev_t *device)
{
	return device->address_range_length;

}

bool WS_get_device_has_address(WS_dev_t *device)
{
	return device->has_address;
}


WS_dev_t *WS_device_init(WS_dev_desc_t *desc, device_command_t *cmd)
{
	static int devid = 0;
	WS_dev_t *device = calloc(1, sizeof(WS_dev_t));

	device->desc = desc;
	device->mailbox = WS_device_mailbox_init();
	device->ptr = desc->vtable->init(device);
	device->id = devid++;
	WS_device_cmd(device, cmd);

	return device;
}


//update devices
void WS_device_update(WS_dev_t *device)
{
	assert(device->desc->vtable->update);
	device->desc->vtable->update(device);
}


bool WS_device_read(WS_dev_t *device, WS_dev_msg_t *msg)
{
	assert(device->desc->vtable->read);

	return device->desc->vtable->read(device,msg);

}
bool WS_device_send(WS_dev_t *device, WS_dev_msg_t **msg)
{
	assert(device->desc->vtable->send);

	return device->desc->vtable->send(device,msg);
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
	return device->error;
}


