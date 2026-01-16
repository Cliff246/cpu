#include "SIM_wire.h"
#include "OBJ_constants.h"
#include "commons.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

SIM_wire_config_end_t SIM_wire_config_end_init(uint16_t oid, uint8_t cid)
{

	//assert(entry->port_count >= port_id && "entry port count must be greater than selected id");

	SIM_wire_config_end_t end =
	{
		.oid = oid,
		.cid = cid,
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
	assert(wire_config->ends_size < UINT8_MAX);
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


	const uint64_t basic_size = wire_config->ends_size * 2;

	uint16_t map[basic_size];
	const int scale = sizeof(uint16_t);

	memset(map, 0, scale * basic_size);
	for(uint32_t i = 0; i < wire_config->ends_size; ++i)
	{
		uint16_t entry = wire_config->ends[i].oid;
		uint64_t pos = (uint64_t)((uint16_t)entry % basic_size);
		bool found = false;
		for(uint32_t p = pos, c = 0; c < basic_size; p++, c++)
		{
			if(p == basic_size)
			{
				p = 0;
			}
			if(map[p] == 0)
			{
				map[p] = entry;
				found = true;
				break;
			}
			else if(map[p] == entry)
			{
				assert(0 && "cannot over lap wires");

			}
		}
		if(found == false)
		{
			assert(0);
		}

	}



	wire_config->flags.set = true;
	return true;
}


SIM_wire_t SIM_wire_init(uint32_t channel_start, uint32_t channel_length, uint32_t slot_start, uint32_t slot_length)
{
	assert(channel_length < OBJ_MAX_CHANNELS);
	SIM_wire_t wire =
	{
		.channel_start = channel_start,
		.channel_length = channel_length,
		.slot_start = slot_start,
		.slot_length = slot_length,
		.cur_scroll = 0,
		.cur_len = 0,
	};
	return wire;
}