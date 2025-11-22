#include "device_type_ptr.h"

#include "device.h"
#include <stdlib.h>

#include "dev_fakeio.h"
#include "dev_ram.h"
#include "dev_wcpu.h"

device_class_t device_vtable[DEVICES_TYPE_COUNT] =
{
	[DEVICE_INVAL] = {.init = NULL, .update = NULL,.read = NULL, .send = NULL},
	[DEVICE_FAKEIO] = {.init = device_fakeio_generate, .update = device_fakeio_update,.read = NULL, .send= NULL},
	[DEVICE_WCPU] = {.init = device_wcpu_generate, .update = device_wcpu_update,.read = NULL, .send = NULL},
	[DEVICE_RAM] = {.init = device_ram_generate, .update = device_ram_update,.read = NULL, .send = NULL},
};


device_t *device_generate(emuconfig_dev_settings_t *settings )
{

	device_t *device = calloc(1, sizeof(device_t));
	device_type_t type = settings->type;
	device->type = type;
	device->device = device_vtable[type].init(device, settings);

	return device;
}




void device_update(device_t *device)
{

}