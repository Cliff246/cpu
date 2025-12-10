#ifndef __FAKEIO_HEADER__
#define __FAKEIO_HEADER__


#include <stdint.h>
#include "device.h"
#include "device_list.h"
#include "emulator_configure.h"
#include "dev_fakeio_device_config_setting.h"

typedef struct device_fakeio
{

}dev_fakeio_t;


device_type_ptr_t device_fakeio_generate(device_t *device,emuconfig_dev_settings_t *settings);
void device_fakeio_update(device_t *device);
bool device_fakeio_read(device_t *dev, dev_msg_t *msg);
dev_msg_t *device_fakeio_send(device_t *dev);
void device_fakeio_print(device_t *dev);
void device_fakeio_cmd(device_t *dev, device_command_t *cmd);

#endif