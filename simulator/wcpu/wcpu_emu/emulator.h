#ifndef __EMULATOR_CORE__
#define __EMULATOR_CORE__

#include "device.h"
#include "emulator_configure.h"
#include <stdlib.h>
#include <stdbool.h>

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
	int stable_slots;


}emulator_t;


device_t *emulator_get_device_from_id(emulator_t *emu, dev_id_t devid);
bool emulator_get_device_from_address(emulator_t *emulator, device_t **dev, size_t address);

static void emulator_sort_slots(emulator_t *emu);
emulator_t *emulator_generate(emuconfig_t *config);



void emulator_update(emulator_t *emu);
void emulator_print_slots(emulator_t *emu);
static void print_slot(emu_dev_slot_t *slot, int index);

//todo



#endif
