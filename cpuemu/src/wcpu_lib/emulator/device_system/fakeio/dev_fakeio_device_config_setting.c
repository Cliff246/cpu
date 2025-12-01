#include "dev_fakeio_device_config_setting.h"
#include "device.h"
#include "device_list.h"
#include <stdlib.h>
#include <assert.h>

device_config_setting_ptr_t device_init_config_setting_fakeio(device_type_t type)
{

	dev_fakeio_config_setting_t *config_setting = calloc(1, sizeof(dev_fakeio_config_setting_t));
	assert(config_setting != NULL);
	device_config_setting_ptr_t ptr;
	ptr.fakeio = config_setting;
	return ptr;

}

void device_free_config_setting_fakeio(device_config_setting_ptr_t ptr)
{
	free(ptr.fakeio);
}