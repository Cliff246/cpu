#include "emucore.h"

#include <stdlib.h>
#include <stdio.h>

emulator_t *emulator_generate(emuconfig_t *config)
{

	emulator_t *emu = calloc(1, sizeof(emu));

	if(emu == NULL)
	{
		perror("emulator was not created");
		exit(EXIT_FAILURE);
	}




	emu->config = config;

	emu->device_list = calloc(config->settings_index, sizeof(device_t *));
	if(emu->device_list == NULL)
	{
		perror("could not create device list");
		exit(EXIT_FAILURE);
	}

	emu->device_count = config->settings_index;

	emu->device_slots = calloc(emu->device_count, sizeof(emu_dev_slot_t));

	if(emu->device_slots == NULL)
	{
		perror("could not create device slots");
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < emu->device_count; ++i)
	{
		if(emu->config->settings[i].used == true)
		{
			emu->device_list[i] = device_generate(emu->config->settings[i].type);
		}
	}






	return emu;

}


static int emulator_device_address_search(emulator_t *emulator, size_t address)
{
	size_t low = 0;
	size_t high = emulator->device_count - 1;
	size_t mid = 0;
	while(low <= high)
	{
		mid = low + (high - low) / 2;

		size_t mid_value = emulator->device_slots[mid].address_start;
		if(mid_value == address)
			return mid;
		if(mid_value < address)
			low = mid + 1;
		else
			high = mid - 1;
	}
	return mid;
}


device_t *emulator_device_by_address(emulator_t *emulator, size_t address)
{







}

//todo
void peek_system(char *cmd);