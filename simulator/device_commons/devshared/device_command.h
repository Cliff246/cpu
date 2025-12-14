#ifndef __DEVICE_COMMAND_HEADER__
#define __DEVICE_COMMAND_HEADER__


#include "device_list.h"
#include <stdbool.h>


typedef struct device_command
{
	//used to expire this device ptr
	bool used;
	device_type_t type;
	void *settings;
}device_command_t;



typedef void*(*device_config_setting_init_fn)(device_type_t type);
typedef void(*device_config_setting_free_fn)(void *);

typedef struct device_config_setting_vtable
{
	device_config_setting_init_fn init;
	device_config_setting_free_fn free;
}device_config_setting_vtable_t;




#endif