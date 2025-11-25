#include "dev_wcpu.h"
#include "core.h"
#include <stdlib.h>

device_type_ptr_t device_wcpu_generate(device_t *device,emuconfig_dev_settings_t *settings)
{
	dev_wcpu_t *cpu = calloc(1, sizeof(dev_wcpu_t));
	cpu->core = wcpu_core_generate();

	device_type_ptr_t ptr;
	ptr.wcpu = cpu;
	return ptr;

}

void device_wcpu_update(device_t *device)
{
	wcpu_core_update(device->device.wcpu->core);
}

void device_wcpu_read(device_t *dev, dev_msg_t *send)
{
	
}


dev_msg_t *device_wcpu_send(device_t *dev)
{
	return NULL;
}
