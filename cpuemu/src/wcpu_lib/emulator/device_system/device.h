#ifndef __DEVICE_HEADER__
#define __DEVICE_HEADER__


#include "device_type_ptr.h"


typedef struct device_class device_class_t;


typedef struct device
{
	device_type_t type;
	int error;
	//timers
	int timer_current;
	int timer_numerator;
	int timer_denominator;
	//device ptr
	device_type_ptr_t device;
}device_t;

typedef device_t *(*init_device)(void);
typedef void (*step_device)(device_t *device);

typedef struct device_class
{
	init_device init;
	step_device update;

}device_class_t;

extern device_class_t device_vtable;

device_t *device_generate(device_type_t type);

void device_update(device_t *device);



#endif