#ifndef __DEV_FAKEIO_DEVICE_CONFIG_SETTING_HEADER__
#define __DEV_FAKEIO_DEVICE_CONFIG_SETTING_HEADER__

#include "device_list.h"


typedef struct device_fakeio_config_setting
{

} dev_fakeio_config_setting_t;

void *device_init_config_setting_fakeio(device_type_t type);
void device_free_config_setting_fakeio(void *ptr);

#endif
