#ifndef __DEVICE_WCPU_EMULATOR_CONFIG_SETTING_HEADER__
#define __DEVICE_WCPU_EMULATOR_CONFIG_SETTING_HEADER__

#include "device_list.h"

typedef struct device_wcpu_config_setting
{

}dev_wcpu_config_setting_t;

void *device_init_config_setting_wcpu(device_type_t type);
void device_free_config_setting_wcpu(void *ptr);


#endif