#ifndef __DEVICE_WCPU__
#define __DEVICE_WCPU__

#include "device_list.h"
#include "device.h"
#include "core.h"
#include "device_message.h"

#include <stdint.h>
#include <stdbool.h>



typedef struct device_wcpu
{

	core_t *core;

	int port_behaviour;

	bool has_out, has_in;
	dev_msg_t *current_msg_out;
	dev_msg_t *current_msg_in;
}dev_wcpu_t;


device_type_ptr_t device_wcpu_generate(device_t *device, emuconfig_dev_settings_t *settings);

void device_wcpu_update(device_t *dev);

void device_wcpu_read(device_t *dev, dev_msg_t *send);

dev_msg_t *device_wcpu_send(device_t *dev);
void device_wcpu_print(device_t *dev);


#endif
