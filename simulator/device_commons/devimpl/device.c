#include "device_list.h"

#include <stdlib.h>
#include <assert.h>
#include "device.h"
#include "device_commons.h"


char *device_type_str[] =
{

};

size_t get_device_address_start(device_t *device)
{

}

size_t get_device_address_length(device_t *device)
{

}



dev_mailbox_t *get_device_mailbox(device_t *dev)
{

}



WS_dev_t *device_init(device_command_t *cmd)
{

}


bool get_device_has_address(device_t *device)
{
}

void device_update(device_t *device)
{


}

bool device_read(device_t *device, dev_msg_t *msg)
{



}


dev_msg_t *device_send(device_t *device)
{
}




void device_print(device_t *device)
{

}


void device_cmd(device_t *device, device_command_t *cmd)
{

}


bool get_device_changed(device_t *device)
{
	assert(device != NULL && "cannot update null device");
	return device->flags[DEVICE_FLAG_TYPE_INTERNAL_CHANGED];
}

//only sets to high, device manager should clear
void set_device_changed(device_t *device)
{
	assert(device != NULL && "cannot update null device");
	device->flags[DEVICE_FLAG_TYPE_INTERNAL_CHANGED] = true;

}
