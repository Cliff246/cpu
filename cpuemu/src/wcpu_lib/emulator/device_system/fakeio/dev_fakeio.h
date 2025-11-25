#ifndef __FAKEIO_HEADER__
#define __FAKEIO_HEADER__


#include <stdint.h>
#include "device.h"
#include "device_type_ptr.h"
#include "emulator_configure.h"

typedef struct device_fakeio
{

}dev_fakeio_t;


device_type_ptr_t device_fakeio_generate(device_t *device,emuconfig_dev_settings_t *settings);
void device_fakeio_update(device_t *device);
void device_fakeio_read(device_t *dev, dev_msg_t *);
dev_msg_t *device_fakeio_send(device_t *dev);


#endif