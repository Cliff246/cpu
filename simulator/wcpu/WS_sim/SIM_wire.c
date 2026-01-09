#include "SIM_wire.h"
#include "commons.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <assert.h>

SIM_wire_config_end_t SIM_wire_config_end_init(SIM_entry_t *entry, uint32_t port_id)
{

	assert(entry->port_count >= port_id && "entry port count must be greater than selected id");

	SIM_wire_config_end_t end =
	{
		.entry = entry,
		.port = port_id
	};
	return end;
}

SIM_wire_config_t *SIM_wire_config_init(uint32_t delay)
{
	SIM_wire_config_t *config = calloc(1, sizeof(SIM_wire_config_t));
	assert(delay != 0 && "cannot have non existant delay");
	assert(config != NULL);
	config->delay = delay;
	config->flags.set = false;
	config->ends_size = 0;
	config->ends = NULL;
	return config;
}

void SIM_wire_config_add_end(SIM_wire_config_t *wire_config, SIM_wire_config_end_t end)
{
	assert(wire_config != NULL);
	wire_config->ends = realloc_safe(wire_config->ends, wire_config->ends_size + 1, sizeof(SIM_wire_config_end_t));

	wire_config->ends[wire_config->ends_size++] = end;
	return;
}

bool SIM_wire_config_set(SIM_wire_config_t *wire_config)
{
	if(wire_config == NULL)
	{
		return false;
	}
	if(wire_config->delay == 0)
	{
		return false;
	}
	if(wire_config->ends_size <= 1)
	{
		return false;
	}
	if(wire_config->flags.set == true)
	{
		return false;
	}

	/*
	uint64_t basic_size = wire_config->ends_size * 2;
	int *ptr = calloc(basic_size, sizeof(int));

	to lazy to implement a dictonary check, dont over lap elements

	for(uint32_t i = 0; i < wire_config->ends_size; ++i)
	{
		uint32_t pos = (uint32_t)((uint64_t)wire_config->ends[i].entry % basic_size);

		ptr[pos]++;
	}

	*/


	wire_config->flags.set;
	return true;
}


SIM_wire_t SIM_wire_init(uint32_t channel_start, uint32_t channel_length, uint32_t slot_start, uint32_t slot_length)
{
	SIM_wire_t wire =
	{
		.channel_start = channel_start,
		.channel_length = channel_length,
		.slot_start = slot_start,
		.slot_length = slot_length,
		.current_scroll = 0
	};
	return wire;
}