#include "SIM_wire.h"
#include "OBJ_constants.h"
#include "SIM_channel.h"
#include "SIM_commons.h"
#include "SIM_graph.h"
#include "SIM_mail.h"
#include "SIM_port.h"
#include "SIM_wirerouter.h"
#include "commons.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>


bool SIM_init_wire(SIM_wire_t *wire, SIM_wirecfg_t *cfg)
{


	wire->id = cfg->id;
	SIM_init_wirering(&wire->wirering, cfg->latency);
	SIM_init_router(&wire->router, cfg->channels_count);



	return wire;
}

void SIM_print_wire(SIM_wire_t *wire)
{
	for(int i = 0; i < wire->router.size; ++i)
	{
		//printf("print: %d %d\n", i, wire->size);

	}
}

/*
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
	wire->bus_rr_last = (wire->bus_rr_last >= wire->channel_length)? 0 : wire->bus_rr_last + 1;
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



	SIM_bus_t *bus = &graph->buses[wire->wire_index];

	assert(bus->bus_index == wire->wire_index);
	uint64_t address = channel->address;
	SIM_tag_t tag = SIM_bus_find_address(bus, address);
	assert(tag > 0);
	int16_t local = SIM_bus_find_local_channel(bus,tag);
	assert(local < wire->channel_length);
	if(local < 0)
		assert(0);
	wire->transfer_channel_output = local;
	wire->transfering = true;
	return true;
}


SIM_wire_channel_t SIM_wire_bus_get_input(SIM_wire_t *wire)
{
	assert(wire->transfering == true);
	return wire->transfer_channel_input;
}

SIM_wire_channel_t SIM_wire_bus_get_output(SIM_wire_t *wire)
{
	assert(wire->transfering == true);

	return wire->transfer_channel_output;

}

SIM_transfer_global_t SIM_wire_get_current_transfer_global(SIM_wire_t *wire)
{

	assert(wire->transfering == true);

	return wire->transfer_start + wire->current_transfer_scroll;

}
*/