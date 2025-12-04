#include "dev_fakeio.h"
#include <stdlib.h>

device_type_ptr_t device_fakeio_generate(device_t *device, emuconfig_dev_settings_t *settings)
{
	dev_fakeio_t *fakeio = calloc(1, sizeof(dev_fakeio_t));

	device_type_ptr_t ptr;
	ptr.fakeio = fakeio;
	return ptr;
}

//TDOO
void device_fakeio_update(device_t *device)
{

}

//TODO
void device_fakeio_read(device_t *dev, dev_msg_t *)
{

}

//TODO
dev_msg_t *device_fakeio_send(device_t *dev)
{
	return NULL;
}

void device_fakeio_print(device_t *dev)
{

}

void device_fakeio_cmd(device_t *dev, device_command_t *cmd)
{

}