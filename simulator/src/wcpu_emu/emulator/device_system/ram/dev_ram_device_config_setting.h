#ifndef __DEVICE_RAM_EMULATOR_CONFIG_SETTING_HEADER__
#define __DEVICE_RAM_EMULATOR_CONFIG_SETTING_HEADER__

#include "device_list.h"
#include "commons.h"
#include <stdint.h>
#include <stdbool.h>

#define DEVICE_RAM_CONFIG_SETTING_ENABLE_LIST(X)\
	X(FILENAME)									\
	X(SIZE)										\
	X(START)									\
	X(PRINT_CONTENT)							\
	X(RESET)									\

#define DEVICE_RAM_CONFIG_SETTING_ENABLE_NAME(X) DEVICE_RAM_CONFIG_SETTING_ENABLE_FLAG_ ## X

#define DEVICE_RAM_CONFIG_SETTING_ENABLE_ENUM(X) DEVICE_RAM_CONFIG_SETTING_ENABLE_NAME(X),
#define DEVICE_RAM_CONFIG_SETTING_ENABLE_COUNT (0 + DEVICE_RAM_CONFIG_SETTING_ENABLE_LIST(COUNTER1D))


typedef enum device_ram_config_setting_enable
{
	DEVICE_RAM_CONFIG_SETTING_ENABLE_LIST(DEVICE_RAM_CONFIG_SETTING_ENABLE_ENUM)
}ram_config_setting_enable_t;



struct device_ram_config_setting
{
	//file name flag
	char *filename;
	//size flag
	uint64_t size;
	uint64_t start;
	//size print content flag
	bool print_content;
	bool settings[DEVICE_RAM_CONFIG_SETTING_ENABLE_COUNT];
};

device_config_setting_ptr_t device_init_config_setting_ram(device_type_t type);
void device_free_config_setting_ram(device_config_setting_ptr_t ptr);



#endif