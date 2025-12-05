#include "device_list.h"

#include "device.h"
#include <stdlib.h>
#include <assert.h>

#include "dev_fakeio.h"
#include "dev_ram.h"
#include "dev_wcpu.h"

device_class_t device_vtable[DEVICES_LIST_TYPE_COUNT] =
{
	[DEVICE_INVAL] = {.init = NULL, .update = NULL,.read = NULL, .send = NULL, .print = NULL},

	[DEVICE_FAKEIO] = {
		.init = device_fakeio_generate,
		.update = device_fakeio_update,
		.read = device_fakeio_read,
		.send= device_fakeio_send,
		.print = NULL,
		.cmd = device_fakeio_cmd,
	},

	[DEVICE_WCPU] = {
		.init = device_wcpu_generate,
		.update = device_wcpu_update,
		.read = device_wcpu_read,
		.send = device_wcpu_send,
		.print = device_wcpu_print,
		.cmd = device_wcpu_cmd,

	},

	[DEVICE_RAM] = {
		.init = device_ram_generate,
		.update = device_ram_update,
		.read = device_ram_read,
		.send = device_ram_send,
		.print = device_ram_print,
		.cmd = device_ram_cmd,

	},
};


char *device_type_str[] =
{
	"INVAL",
	DEVICE_LIST(DEVICE_TYPE_ENUM_AS_STRING)
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

	for(int i = 0; i < DEVICE_FLAG_COUNT; ++i)
	{
		device->flags[i] = false;
	}


	assert(settings->command != NULL && "settings command cannot be null");





	device_type_t type = settings->command->type;
	device->type = type;
	//printf("%d\n", device->type);
	assert(device_vtable[type].init != NULL && "cannot init a device with no vtable fn");
	device->device = device_vtable[type].init(device, settings);

	assert(device->device.ptr != NULL && "device ptr cannot be null");

	device->flags[DEVICE_FLAG_TYPE_ACTIVATED] = true;

	device->mailbox = device_mailbox_init();
	//set_device_changed(device);



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

	assert(get_device_changed(device) == false && "device changed on update is not valid, must handle before");

	//this is boring but yay
	device_vtable[device->type].update(device);

}

bool device_read(device_t *device, dev_msg_t *msg)
{
	assert(device != NULL && "cannot update null device");
	assert(device->type != DEVICE_INVAL && "cannot update device_inval");


	assert(device_vtable[device->type].read != NULL && "device doesnt have a read function");
	assert(get_device_changed(device) == false && "device changed on read is not valid, must handle before");

	device_vtable[device->type].read(device, msg);

	device_message_release(&msg);


}


dev_msg_t *device_send(device_t *device)
{
	assert(device != NULL && "cannot update null device");
	assert(device->type != DEVICE_INVAL && "cannot update device_inval");


	assert(device_vtable[device->type].send != NULL && "device doesnt have a read function");
	assert(get_device_changed(device) == false && "device changed on send is not valid, must handle before");

	dev_msg_t *new_msg = device_vtable[device->type].send(device);
	if(new_msg == NULL)
		return NULL;

	return new_msg;
}




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


void device_cmd(device_t *device, device_command_t *cmd)
{
	assert(device != NULL && "cannot update null device");
	assert(device->type != DEVICE_INVAL && "cannot update device_inval");


	assert(device_vtable[device->type].cmd != NULL && "device doesnt have a read function");

	device_vtable[device->type].cmd(device, cmd);
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