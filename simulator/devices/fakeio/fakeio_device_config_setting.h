#ifndef __DEV_FAKEIO_DEVICE_CONFIG_SETTING_HEADER__
#define __DEV_FAKEIO_DEVICE_CONFIG_SETTING_HEADER__

#include "device_list.h"
#include "device_description.h"

typedef struct device_fakeio_config_setting
{

} dev_fakeio_config_setting_t;

void *device_init_config_setting_fakeio(WS_dev_desc_t *desc);
void device_free_config_setting_fakeio(void *ptr);

#endif
