#ifndef __DEVICE_HEADER__
#define __DEVICE_HEADER__

#include "dev_wcpu.h"
#include "dev_ram.h"
#include "dev_fakeio.h"

typedef enum device_type
{
	DEVICE_INVAL,
	DEVICE_WCPU,
	DEVICE_FAKEIO,
	DEVICE_DEBUGER,
	DEVICE_RAM,
}device_type_t;

typedef struct device
{
	device_type_t type;

	int error;

	void *ptr;
}device_t;

typedef device_t *(*init_device)(device_type_t type);
typedef void (*step_device)(device_t *device);

typedef struct device_class
{
	init_device init;
	step_device update;

}device_class_t;


device_t *device_generate(device_type_t type);

void device_update(device_t *device);



#endif