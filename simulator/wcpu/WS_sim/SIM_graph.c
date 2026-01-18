#include "SIM_graph.h"
#include "OBJ_bundle.h"
#include "OBJ_constants.h"
#include "OBJ_message.h"
#include "SIM_channel.h"
#include "SIM_commons.h"
#include "SIM_object.h"
#include "SIM_port.h"
#include "SIM_transfer.h"
#include "SIM_wire.h"
#include "commons.h"
#include "SIM_routetable.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

SIM_graph_t *SIM_graph_init(void)
{

	SIM_graph_t *graph = calloc(1, sizeof(SIM_graph_t));
	assert(graph);


	return graph;
}

//TODO fix this, make it spawn a template graph
SIM_graph_t *SIM_graph_init_template(void)
{
	SIM_graph_t *graph = SIM_graph_init();
	assert(0 && "TODO");
	return graph;
}

/*
void SIM_graph_add_object(SIM_graph_t *graph)
{
	assert(graph->flags.set != true );

	graph->objects = realloc_safe(graph->objects, graph->objects_size, sizeof(SIM_object_t ));

	graph->objects[graph->objects_size++] = object;
	graph->flags.changed = true;
	graph->flags.set = false;
}
*/

void SIM_graph_set_changed(SIM_graph_t *graph)
{
	assert(graph && "graph cannot be null");

	graph->flags.changed = true;
	graph->flags.set = false;
}

void SIM_graph_add_wire(SIM_graph_t *graph, SIM_wire_config_t *wire)
{
	assert(graph->flags.set != true );
	assert(wire->flags.set == true);
	graph->config.wire_configs = realloc_safe(graph->config.wire_configs, graph->config.wire_configs_size + 1, sizeof(SIM_wire_config_t *));
	graph->config.wire_configs[graph->config.wire_configs_size++] = wire;

	SIM_graph_set_changed(graph);
}


bool SIM_graph_set(SIM_graph_t *graph)
{

	assert(graph);
	assert(graph->flags.set == false);


	assert(graph->objects_size >= 2);
	assert(graph->config.wire_configs_size >= 1);
	assert(graph->objects);


	uint64_t wireslot_size = 0;
	uint64_t channel_size = 0;


	//sum up the amount of wireslots needed and channel size
	for(int id = 0; id < graph->config.wire_configs_size; ++id)
	{
		SIM_wire_config_t *cfg = graph->config.wire_configs[id];
		wireslot_size += cfg->delay;
		channel_size += cfg->ends_size;
	}

	//graph->wireslots_size = wireslot_size;
	graph->wires_size = graph->config.wire_configs_size;
	graph->channels_size = channel_size;
	graph->channels = calloc(graph->channels_size, sizeof(SIM_channel_t));
	assert(graph->channels && "graph channels alloc");
	graph->wires = calloc(graph->wires_size, sizeof(SIM_wire_t));
	assert(graph->wires && "graph wires alloc");
	//graph->wireslots = calloc(graph->wireslots_size, sizeof(SIM_wireslot_t));
	//assert(graph->wireslots && "graph wireslots alloc");
	graph->ports_size = graph->objects_size;
	graph->ports = calloc(graph->ports_size, sizeof(SIM_port_t));
	assert(graph->ports && graph->ports_size);
	uint32_t channel_count = 0;
	uint32_t wireslot_count = 0;
	const uint32_t wires_size =  graph->wires_size;
	for(int iw = 0; iw < wires_size; ++iw)
	{
		SIM_wire_config_t *cfg = graph->config.wire_configs[iw];
		uint64_t tmpchannel_size =  cfg->ends_size;
		uint64_t tmpchannel = channel_count;
		for(int ie = 0; ie < cfg->ends_size; ++ie)
		{
			SIM_wire_config_end_t end =  cfg->ends[ie];
			int64_t entry_postion = end.oid;
			if(entry_postion == -1)
			{
				//this is crude and does not allow one wire to connect to multiple ports
				assert(0 && "cannot miss entry found in wire end");
			}

			 SIM_channel_init(&graph->channels[channel_count++],entry_postion, end.cid);
		}


		graph->wires[iw] = SIM_wire_init(tmpchannel, tmpchannel_size, wireslot_count, cfg->delay);
		wireslot_count += cfg->delay;
	}


	graph->flags.changed = false;
	graph->flags.set = true;
	return true;
}



//assumes an open routetable
//returns -1 on failure and on success returns an index in an object channels
int16_t SIM_graph_find_channel_open_routetable(SIM_graph_t *graph, SIM_object_global_t oid, uint8_t route)
{
	SIM_object_t *obj = &graph->objects[oid];
	const uint16_t port_id = obj->port_index;
	SIM_port_t *port = &graph->ports[port_id];
	SIM_routetable_t *rt = &port->routetable;

	SIM_route_t *route_ptr = &rt->table[route];


	//iterator overlap
	for(uint16_t io = 0; io < route_ptr->overlap_length; ++io)
	{
		//route channel
		const uint8_t route_chnl = rt->overlap[route_ptr->overlap_start + io];
		assert(route_chnl < OBJ_MAX_CHANNELS);
		//const uint32_t port_chnl_id = port->channels[route_chnl];
		//SIM_channel_t *channel = &graph->channels[port_chnl_id];



		assert(0);
		{
			return route_chnl;
		}
	}
	return -1;
}



void SIM_graph_object_update(SIM_graph_t *graph)
{

	const SIM_object_global_t objects_size = graph->objects_size;

	for(uint32_t i = 0; i < objects_size; ++i)
	{

		SIM_object_t *object = &graph->objects[i];

		SIM_object_update(object);

	}
}

void SIM_graph_object_read(SIM_graph_t *graph)
{
	const SIM_object_global_t objects_size = graph->objects_size;

	for(uint32_t i = 0; i < objects_size; ++i)
	{

		SIM_object_t *object = &graph->objects[i];

		OBJ_bundle_t bnd = {0};
		assert(0 && "TODO");

		SIM_object_read(object, &bnd);

	}
}

void SIM_graph_object_write(SIM_graph_t *graph)
{
	const SIM_object_global_t objects_size = graph->objects_size;

	for(uint32_t i = 0; i < objects_size; ++i)
	{

		SIM_object_t *object = &graph->objects[i];



		OBJ_bundle_t bnd = {0};
		assert(0 && "TODO");

		SIM_object_write(object, &bnd);
		//todo gather bundle information send to port
	}
}

void SIM_graph_update(SIM_graph_t *graph)
{

	assert(graph->flags.changed == false);

	const uint32_t wire_size = graph->wires_size;

	//iwr = iterator wire read
	for(uint32_t iwr = 0; iwr < wire_size; ++iwr)
	{
		//tgt:import

	}

	SIM_graph_object_read(graph);
	SIM_graph_object_update(graph);
	SIM_graph_object_write(graph);

	//write output and shit
	for(uint32_t iww = 0; iww < wire_size; ++iww)
	{

		// Arbitration rule (UNDEFINED):
		// Select next src/tgt channel pair for this wire.
		// Must ensure progress without starvation.
		// Must not violate channel ownership invariants.
		SIM_wire_t *wire = &graph->wires[iww];

		//SIM_


	}
}
