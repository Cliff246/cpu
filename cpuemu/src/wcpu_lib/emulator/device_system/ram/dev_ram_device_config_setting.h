#ifndef __DEVICE_RAM_EMULATOR_CONFIG_SETTING_HEADER__
#define __DEVICE_RAM_EMULATOR_CONFIG_SETTING_HEADER__

#include "device_list.h"

#include <stdint.h>
#include <stdbool.h>

struct device_ram_config_setting
{
	bool use_filename;
	char *filename;
	bool use_size;
	int64_t size;

};

device_config_setting_ptr_t device_init_config_setting_ram(device_type_t type);
void device_free_config_setting_ram(device_config_setting_ptr_t ptr);



#endif