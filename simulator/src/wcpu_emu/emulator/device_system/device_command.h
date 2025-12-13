#ifndef __DEVICE_COMMAND_HEADER__
#define __DEVICE_COMMAND_HEADER__


#include "device_list.h"
#include <stdbool.h>


typedef struct device_command
{
	//used to expire this device ptr
	bool used;
	device_type_t type;
	device_config_setting_ptr_t setting;
}device_command_t;

device_command_t *device_command_generate(device_type_t type);
void device_command_free(device_command_t *command);



typedef device_config_setting_ptr_t(*device_config_setting_init_fn)(device_type_t type);
typedef void(*device_config_setting_free_fn)(device_config_setting_ptr_t ptr);

typedef struct device_config_setting_vtable
{
	device_config_setting_init_fn init;
	device_config_setting_free_fn free;
}device_config_setting_vtable_t;


extern device_config_setting_vtable_t device_config_setting_vtable_array[];


device_config_setting_ptr_t device_init_config_setting(device_type_t type);
void device_free_config_setting(device_config_setting_ptr_t ptr, device_type_t type);


#endif