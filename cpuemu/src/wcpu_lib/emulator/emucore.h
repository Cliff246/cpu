#ifndef __EMULATOR_CORE__
#define __EMULATOR_CORE__

#include "device.h"
#include "device_configurer.h"
#include "emulator_configure.h"

typedef struct emulator
{
	emuconfig_t *config;
	device_t **device_list;
	int device_count;


}emulator_t;

emulator_t *emulator_generate(emuconfig_t *config);

//todo



#endif
