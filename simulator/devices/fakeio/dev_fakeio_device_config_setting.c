#include "dev_fakeio_device_config_setting.h"
#include "device.h"
#include "device_list.h"
#include <stdlib.h>
#include <assert.h>

void *device_init_config_setting_fakeio(device_type_t type)
{

	dev_fakeio_config_setting_t *config_setting = calloc(1, sizeof(dev_fakeio_config_setting_t));
	assert(config_setting != NULL);

	return (void *)config_setting;

}

void device_free_config_setting_fakeio(void *ptr)
{
	free(ptr);
}