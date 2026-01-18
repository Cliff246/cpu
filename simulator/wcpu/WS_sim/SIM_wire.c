#include "SIM_wire.h"
#include "OBJ_constants.h"
#include "SIM_channel.h"
#include "SIM_commons.h"
#include "SIM_graph.h"
#include "SIM_object.h"
#include "SIM_port.h"
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

SIM_wire_t SIM_wire_init(uint32_t channel_start, uint32_t channel_length, uint32_t transfer_start, uint32_t transfer_length)
{
	assert(channel_length < OBJ_MAX_CHANNELS);
	SIM_wire_t wire =
	{
		.channel_start = channel_start,
		.channel_length = channel_length,
		.transfer_start = transfer_start,
		.transfer_length = transfer_length,
		.current_transfer_scroll = 0,
		.current_transfer_length = 0,
	};
	return wire;
}


inline void SIM_wire_update_scroll(SIM_wire_t *wire)
{
	assert(wire);

	wire->current_transfer_scroll = (wire->current_transfer_scroll >= wire->transfer_length)? 0 : wire->current_transfer_scroll + 1;

}

bool SIM_wire_bus_used_channel_roundrobin(SIM_wire_t *wire, SIM_wire_channel_t local)
{
	assert(local < OBJ_MAX_CHANNELS);
	uint32_t i = wire->bus_rr_used;
	bool bit = GETBIT(i, local);
	return bit;
}

void SIM_wire_bus_set_used_channel_roundrobin(SIM_wire_t *wire, SIM_wire_channel_t local)
{
	assert(local < OBJ_MAX_CHANNELS);

	assert(SIM_wire_bus_used_channel_roundrobin(wire, local) == false && "cannot set already used bit");
	wire->bus_rr_used = SETBIT(wire->bus_rr_used, local);
}

void SIM_wire_bus_clear_used_channel_roundrobin(SIM_wire_t *wire, SIM_wire_channel_t local)
{
	assert(SIM_wire_bus_used_channel_roundrobin(wire, local) == true && "cannot set already used bit");

	wire->bus_rr_used = CLRBIT(wire->bus_rr_used, local);
}


void SIM_wire_bus_add_channel_roundrobin(SIM_wire_t *wire, SIM_wire_channel_t local)
{
	assert(wire);
	assert(wire->bus_rr_length < wire->channel_length && "cannot have more roundrobin entries then channels");

	assert(SIM_wire_bus_used_channel_roundrobin(wire, local) == false && "cannot add channel already defined");

	const int pos = (wire->bus_rr_last + wire->bus_rr_length) % wire->channel_length;
	wire->bus_rr[pos] = local;
	SIM_wire_bus_set_used_channel_roundrobin(wire, local);
	wire->bus_rr_length++;
}

SIM_wire_channel_t SIM_wire_bus_get_channel_roundrobin(SIM_wire_t *wire)
{
	assert(wire);
	assert(wire->bus_rr_length > 0);
	return wire->bus_rr[wire->bus_rr_last];
}

bool SIM_wire_bus_has_channel_roundrobin(SIM_wire_t *wire)
{
	return (wire->bus_rr_length == 0)? false : true;
}

void SIM_wire_bus_complete_channel_roundrobin(SIM_wire_t *wire)
{
	assert(wire);
	assert(SIM_wire_bus_has_channel_roundrobin(wire) == true && "wire must have round length > 0");
	wire->transfering = false;
	SIM_wire_channel_t channel = wire->bus_rr[wire->bus_rr_last];
	wire->bus_rr_last = (wire->bus_rr_last >= wire->channel_length)? 0 : wire->bus_rr_last++;
	SIM_wire_bus_clear_used_channel_roundrobin(wire, channel);
	wire->bus_rr_length--;
}

SIM_channel_global_t SIM_wire_channel_convert(SIM_wire_t *wire, SIM_wire_channel_t local)
{
	assert(local < wire->channel_length);
	SIM_channel_global_t global = wire->channel_start + (uint32_t)local;
	return global;
}

bool SIM_wire_bus_dequeue_roundrobin(SIM_graph_t *graph, SIM_wire_t *wire)
{
	if(wire->transfering)
		return false;
	if(!SIM_wire_bus_has_channel_roundrobin(wire))
	{
		return false;
	}

	SIM_wire_channel_t source_channel = SIM_wire_bus_get_channel_roundrobin(wire);

	wire->transfer_channel_input = source_channel;

	SIM_channel_global_t global_channel = SIM_wire_channel_convert(wire, source_channel);
	assert(global_channel < graph->channels_size);
	SIM_channel_t *channel = &graph->channels[global_channel];

	SIM_object_global_t global_object = channel->oid;
	assert(global_object < graph->objects_size);
	SIM_object_t *object = &graph->objects[global_object];
	SIM_port_global_t global_port = object->port_index;
	assert(global_port < graph->ports_size);
	SIM_port_t *port = &graph->ports[global_port];
	//port.
}


