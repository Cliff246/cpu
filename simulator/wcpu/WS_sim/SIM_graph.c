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
#include "SIM_routemap.h"

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


		graph->wires[iw] = SIM_wire_init(tmpchannel, tmpchannel_size, wireslot_count, cfg->delay, iw);
		wireslot_count += cfg->delay;
	}


	graph->flags.changed = false;
	graph->flags.set = true;
	return true;
}




SIM_channel_t *SIM_graph_get_channel(SIM_graph_t *graph, SIM_channel_global_t global)
{
	assert(graph);
	assert(global < graph->channels_size);
	return &graph->channels[global];
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

void SIM_graph_wire_read(SIM_graph_t *graph)
{
	const uint32_t wire_size = graph->wires_size;

	for(uint32_t iwr = 0; iwr < wire_size; ++iwr)
	{
		SIM_wire_t *wire = &graph->wires[iwr];
		if(wire->transfering)
		{
			SIM_wire_channel_t output =  SIM_wire_bus_get_output(wire);
			SIM_channel_global_t global_output =  SIM_wire_channel_convert(wire, output);
			SIM_channel_t *channel = SIM_graph_get_channel(graph, global_output);
			SIM_transfer_global_t global_transfer = SIM_wire_get_current_transfer_global(wire);
			channel->packet = SIM_graph_get_transfer(graph, global_transfer)->packet;
		}
	}

}

void SIM_graph_wire_write(SIM_graph_t *graph)
{
	const uint32_t wire_size = graph->wires_size;

	for(uint32_t iww = 0; iww < wire_size; ++iww)
	{
		SIM_wire_t *wire = &graph->wires[iww];
		SIM_wire_update_scroll(wire);
	}
}

void SIM_graph_update(SIM_graph_t *graph)
{

	assert(graph->flags.changed == false);
	SIM_graph_wire_read(graph);
	SIM_graph_object_read(graph);
	SIM_graph_object_update(graph);
	SIM_graph_object_write(graph);
	SIM_graph_wire_write(graph);
}

SIM_transfer_t *SIM_graph_get_transfer(SIM_graph_t *graph, SIM_transfer_global_t global)
{
	assert(global);
	assert(global < graph->transfer_size);
	assert(global >= 0);
	return &graph->transfers[global];
}