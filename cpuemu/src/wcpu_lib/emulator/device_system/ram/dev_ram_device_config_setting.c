#include "dev_ram_device_config_setting.h"
#include "device.h"
#include "device_list.h"
#include <stdlib.h>
#include <assert.h>



device_config_setting_ptr_t device_init_config_setting_ram(device_type_t type)
{

	dev_ram_config_setting_t *config_setting = calloc(1, sizeof(dev_ram_config_setting_t));
	assert(config_setting != NULL);
	device_config_setting_ptr_t ptr;
	ptr.ram = config_setting;
	return ptr;

}

void device_free_config_setting_ram(device_config_setting_ptr_t ptr)
{
	dev_ram_config_setting_t *config_setting = ptr.ram;

	if(config_setting->use_filename)
	{
		free(config_setting->filename);
	}
	free(config_setting);

}