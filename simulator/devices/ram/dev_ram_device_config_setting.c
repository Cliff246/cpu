#include "dev_ram_device_config_setting.h"
#include "device.h"
#include "device_list.h"
#include <stdlib.h>
#include <assert.h>

void *device_init_config_setting_ram(device_type_t type)
{
	assert(type != DEVICE_INVAL && "device cannot be inval");
	dev_ram_config_setting_t *config_setting = calloc(1, sizeof(dev_ram_config_setting_t));
	assert(config_setting != NULL);

	return config_setting;

}


void device_free_config_setting_ram(void *ptr)
{
	assert(ptr);
	dev_ram_config_setting_t *config_setting = (dev_ram_config_setting_t *)ptr;

	if(config_setting->settings[DEVICE_RAM_CONFIG_SETTING_ENABLE_FLAG_FILENAME])
	{
		free(config_setting->filename);
	}
	free(config_setting);

}