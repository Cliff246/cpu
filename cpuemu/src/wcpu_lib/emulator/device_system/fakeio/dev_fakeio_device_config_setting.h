#ifndef __DEV_FAKEIO_DEVICE_CONFIG_SETTING_HEADER__
#define __DEV_FAKEIO_DEVICE_CONFIG_SETTING_HEADER__

#include "device_list.h"


struct device_fakeio_config_setting
{

};

device_config_setting_ptr_t device_init_config_setting_fakeio(device_type_t type);
void device_free_config_setting_fakeio(device_config_setting_ptr_t ptr);

#endif
