#include "dev_fakeio.h"
#include <stdlib.h>
#include <assert.h>

void device_fakeio_generate(device_t *device, device_command_t *cmd )
{
	assert(device);
	dev_fakeio_t *fakeio = calloc(1, sizeof(dev_fakeio_t));
	assert(fakeio);
	device->ptr = fakeio;
}

//TDOO
void device_fakeio_update(device_t *device)
{
	assert(device);
}

//TODO
bool device_fakeio_read(device_t *dev, dev_msg_t *msg)
{
	assert(dev);

}

//TODO
dev_msg_t *device_fakeio_send(device_t *dev)
{
	assert(dev);

	return NULL;
}

void device_fakeio_print(device_t *dev)
{
	assert(dev);

}

void device_fakeio_cmd(device_t *dev, device_command_t *cmd)
{
	assert(dev);

}