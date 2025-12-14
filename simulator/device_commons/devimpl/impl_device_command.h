#ifndef __DEVICE_COMMAND_IMPL_HEADER__
#define __DEVICE_COMMAND_IMPL_HEADER__

#include "device_command.h"


device_command_t *device_command_generate(device_type_t type);
void device_command_free(device_command_t *command);


extern device_config_setting_vtable_t device_config_setting_vtable_array[];


device_config_setting_ptr_t device_init_config_setting(device_type_t type);
void device_free_config_setting(device_config_setting_ptr_t ptr, device_type_t type);

#endif