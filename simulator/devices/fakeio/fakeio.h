#ifndef __FAKEIO_HEADER__
#define __FAKEIO_HEADER__


#include <stdint.h>
#include "fakeio_device_config_setting.h"

#define FAKEIO_BUFFER_SIZE 1000


typedef struct device_fakeio
{
	uint64_t start;
	uint64_t length;
	uint64_t button_address;
	uint64_t fakeio_buffer[FAKEIO_BUFFER_SIZE];
	uint64_t button_value;
	WS_dev_msg_t *msg;
}dev_fakeio_t;




void *device_fakeio_init(device_t *dev);
void device_fakeio_update(device_t *dev);
bool device_fakeio_read(WS_dev_t *dev, WS_dev_msg_t *msg);
bool device_fakeio_send(WS_dev_t *dev, WS_dev_msg_t **msg);
void device_fakeio_print(device_t *dev);
void device_fakeio_commit(WS_dev_t *dev);


#endif