#ifndef __EMULATOR_CORE__
#define __EMULATOR_CORE__

#include "device.h"
#include "device_configurer.h"
#include "bus.h"

typedef struct emulator
{
	device_t *device_list;
	int device_count;

	bus_t *bus;

}emulator_t;

emulator_t *create_emulator(void);

//todo
void peek_system(char *cmd);



#endif
