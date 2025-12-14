#include "device_command.h"
#include "commons.h"


#include <stdlib.h>
#include <assert.h>
/*
device_config_setting_vtable_t device_config_setting_vtable_array[] =
{
	[DEVICE_INVAL] = {
		NULL,
		NULL,
	},

	[DEVICE_WCPU] = {
		.init = device_init_config_setting_wcpu,
		.free = device_free_config_setting_wcpu,
	},

	[DEVICE_FAKEIO] = {
		.init = device_init_config_setting_fakeio,
		.free = device_free_config_setting_fakeio,
	},
	[DEVICE_RAM] = {
		.init = device_init_config_setting_ram,
		.free = device_free_config_setting_ram,
	},
};


device_command_t *device_command_generate(device_type_t type)
{
	assert(type != DEVICE_INVAL && "device is not invalid");
	device_command_t *command = calloc(1, sizeof(device_command_t));

	assert(command != NULL);

	command->type = type;
	command->setting = device_init_config_setting(type);

	assert(command->setting.ptr != NULL);
	command->used = true;
	return command;
}

void device_command_free(device_command_t *command)
{
	//be super safe
	assert(command != NULL);
	//super duper safe
	assert(command->type != DEVICE_INVAL);
	//
	assert(command->used == false);

	device_free_config_setting(command->setting, command->type);

	free(command);

}

device_config_setting_ptr_t device_init_config_setting(device_type_t type)
{

	assert(type != DEVICE_INVAL && "device is not invalid");
	assert(device_config_setting_vtable_array[type].init != NULL);
	return device_config_setting_vtable_array[type].init(type);
}

void device_free_config_setting(device_config_setting_ptr_t ptr, device_type_t type)
{
	assert(type != DEVICE_INVAL && "device is not invalid");

	assert(ptr.ptr);

	assert(device_config_setting_vtable_array[type].free != NULL);

	device_config_setting_vtable_array[type].free(ptr);

}

*/