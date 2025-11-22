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



device_t *device_generate(device_type_t type)
{




	return NULL;
}




void device_update(device_t *device)
{

}