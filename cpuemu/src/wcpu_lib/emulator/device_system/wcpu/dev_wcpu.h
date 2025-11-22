#ifndef __DEVICE_WCPU__
#define __DEVICE_WCPU__

#include "device_type_ptr.h"
#include "device.h"
#include "core.h"
#include "wcpu_part.h"
#include "device_message.h"

#include <stdint.h>


typedef struct device_wcpu
{

	core_t core;

	int port_behaviour;

}dev_wcpu_t;


device_t *device_wcpu_generate(void);

void device_wcpu_update(device_t *device);

dev_msg_t *device_wcpu_read(device_t *dev);
void device_wcpu_write(device_t *dev, dev_msg_t *send);

#endif