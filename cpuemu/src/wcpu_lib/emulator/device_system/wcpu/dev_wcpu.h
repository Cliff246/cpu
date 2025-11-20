#ifndef __DEVICE_WCPU__
#define __DEVICE_WCPU__

#include "device.h"
#include "core.h"
#include "wcpu_part.h"


typedef struct wcpu
{

	core_t core;

	int port_behaviour;

}dev_wcpu_t;


dev_wcpu_t device_wpu_generate(void);

void device_wcpu_update(void *ptr);

#endif