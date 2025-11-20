#ifndef __EMULATOR_CORE__
#define __EMULATOR_CORE__

#include "device.h"
#include "device_configurer.h"

typedef struct emulator
{
	device_t *device_list;
	int device_count;


}emulator_t;

emulator_t *emulator_generate(void);

//todo



#endif
