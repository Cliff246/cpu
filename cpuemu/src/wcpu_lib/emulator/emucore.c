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
	emu->device_count = config->settings_index;
	for(int i = 0; i < emu->device_count; ++i)
	{
		if(emu->config->settings[i].used == true)
		{
			emu->device_list[i] = device_generate(emu->config->settings[i].type);
		}
	}
	







	return emu;

}

//todo
void peek_system(char *cmd);