#ifndef __EMULATOR_CORE__
#define __EMULATOR_CORE__

#include "device.h"
#include "emulator_configure.h"
#include <stdlib.h>

typedef struct emulator_device_slot
{
	//the start and length of devices
	size_t address_start;
	size_t address_length;
	//the device index
	size_t device_index;

	//the device id
	int device_id;
}emu_dev_slot_t;


typedef struct emulator
{
	emuconfig_t *config;
	device_t **device_list;
	emu_dev_slot_t *device_slots;
	int device_count;


}emulator_t;

static void emulator_sort_slots(emulator_t *emu);
emulator_t *emulator_generate(emuconfig_t *config);


device_t *emulator_device_by_address(emulator_t *emulator, size_t address);

void emulator_update(emulator_t *emu);
//todo



#endif
