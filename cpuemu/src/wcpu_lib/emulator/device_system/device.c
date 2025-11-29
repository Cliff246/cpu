#include "device_type_ptr.h"

#include "device.h"
#include <stdlib.h>
#include <assert.h>

#include "dev_fakeio.h"
#include "dev_ram.h"
#include "dev_wcpu.h"

device_class_t device_vtable[DEVICES_TYPE_COUNT] =
{
	[DEVICE_INVAL] = {.init = NULL, .update = NULL,.read = NULL, .send = NULL, .print = NULL},
	[DEVICE_FAKEIO] = {.init = device_fakeio_generate, .update = device_fakeio_update,.read = device_fakeio_read, .send= device_fakeio_send, .print = NULL},
	[DEVICE_WCPU] = {.init = device_wcpu_generate, .update = device_wcpu_update,.read = device_wcpu_read, .send = device_wcpu_send, .print = device_wcpu_print},
	[DEVICE_RAM] = {.init = device_ram_generate, .update = device_ram_update,.read = device_ram_read, .send = device_ram_send, .print = device_ram_print},
};


size_t get_device_address_start(device_t *device)
{
	assert(device != NULL && "device cannot be null");
	return device->address_range_start;
}

size_t get_device_address_length(device_t *device)
{
	assert(device != NULL && "device cannot be null");
	return device->address_range_length;
}



dev_mailbox_t *get_device_mailbox(device_t *dev)
{
	assert(dev != NULL && "device cannot be null");
	assert(dev->mailbox != NULL && "device inbox cannot be null");
	return dev->mailbox;

}





device_t *device_generate(emuconfig_dev_settings_t *settings )
{
	device_t *device = calloc(1, sizeof(device_t));
	device_type_t type = settings->type;
	device->type = type;
	assert(device_vtable[type].init != NULL && "cannot init a device with no vtable fn");
	device->device = device_vtable[type].init(device, settings);
	assert(device->device.ptr != NULL && "device ptr cannot be null");

	device->mailbox = device_mailbox_init();

	return device;
}


bool get_device_has_address(device_t *device)
{
	return device->has_address;
}

void device_update(device_t *device)
{
	assert(device != NULL && "cannot update null device");
	assert(device->type != DEVICE_INVAL && "cannot update device_inval");

	assert(device_vtable[device->type].update != NULL && "cannot update a device with no vtable fn");

	//this is boring but yay
	device_vtable[device->type].update(device);

}

void device_read(device_t *device, dev_msg_t *msg)
{
	assert(device != NULL && "cannot update null device");
	assert(device->type != DEVICE_INVAL && "cannot update device_inval");


	assert(device_vtable[device->type].read != NULL && "device doesnt have a read function");


	device_vtable[device->type].read(device, msg);



}


dev_msg_t *device_send(device_t *device)
{
	assert(device != NULL && "cannot update null device");
	assert(device->type != DEVICE_INVAL && "cannot update device_inval");


	assert(device_vtable[device->type].send != NULL && "device doesnt have a read function");

	dev_msg_t *new_msg = device_vtable[device->type].send(device);

	return new_msg;
}

#define DEVICE_TYPE_ENUM_AS_STRING(X) #X,

static char *device_type_str[] =
{
	DEVICE_TYPE_ENUM(DEVICE_TYPE_ENUM_AS_STRING)
};


void device_print(device_t *device)
{
	printf("devid:%d type:%s\n", (int)device->device_id, device_type_str[device->type]);
	if(device_vtable[device->type].print == NULL)
	{
		printf("no implementation\n");
	}
	else
	{
		device_vtable[device->type].print(device);

	}
}
