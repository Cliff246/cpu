#include "dev_wcpu_device_config_setting.h"
#include "device.h"
#include "device_list.h"
#include <stdlib.h>
#include <assert.h>

void *device_init_config_setting_wcpu(device_type_t type)
{

	dev_wcpu_config_setting_t *config_setting = calloc(1, sizeof(dev_wcpu_config_setting_t));
	assert(config_setting != NULL);

	return config_setting;

}

void device_free_config_setting_wcpu(void *ptr)
{
	free(ptr);

}