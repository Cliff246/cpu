#include "SIM_graph.h"
#include "commons.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

SIM_graph_t *SIM_graph_init(void)
{

	SIM_graph_t *graph = calloc(1, sizeof(SIM_graph_t));
	assert(graph);


	return graph;
}


SIM_graph_t *SIM_graph_init_template(void)
{
	SIM_graph_t *graph = SIM_graph_init();
	return graph;
}


void SIM_graph_add_entry(SIM_graph_t *graph, SIM_entry_t *entry)
{
	assert(graph->flags.set != true );

	graph->entries = realloc_safe(graph->entries, graph->entries_size, sizeof(SIM_entry_t *));

	graph->entries[graph->entries_size++] = entry;
	graph->flags.changed = true;
	graph->flags.set = false;
}

void SIM_graph_add_wire(SIM_graph_t *graph, SIM_wire_config_t *wire)
{
	assert(graph->flags.set != true );
	assert(wire->flags.set == true);
	graph->config.wire_configs = realloc_safe(graph->config.wire_configs, graph->config.wire_configs_size + 1, sizeof(SIM_wire_config_t *));
	graph->config.wire_configs[graph->config.wire_configs_size++] = wire;

	graph->flags.changed = true;
	graph->flags.set = false;
}

int64_t SIM_graph_find_entry(SIM_graph_t *graph, SIM_entry_t *entry)
{
	//yes i did o(n)... hashtables are annoying
	for(uint32_t ie = 0; ie < graph->entries_size; ++ie)
	{
		if(graph->entries[ie] == entry)
		{
			return ie;
		}
	}
	return -1;
}

bool SIM_graph_set(SIM_graph_t *graph)
{

	assert(graph);
	assert(graph->flags.set == false);


	assert(graph->entries_size >= 2);
	assert(graph->config.wire_configs_size >= 1);
	assert(graph->entries);


	uint64_t wireslot_size = 0;
	uint64_t channel_size = 0;

	for(int id = 0; id < graph->config.wire_configs_size; ++id)
	{
		SIM_wire_config_t *cfg = graph->config.wire_configs[id];
		wireslot_size += cfg->delay;
		channel_size += cfg->ends_size;
	}

	graph->wireslots_size = wireslot_size;
	graph->wires_size = graph->config.wire_configs_size;
	graph->channels_size = channel_size;
	graph->channels = calloc(graph->channels_size, sizeof(SIM_channel_t));
	graph->wires = calloc(graph->wires_size, sizeof(SIM_wire_t));
	graph->wireslots = calloc(graph->wireslots_size, sizeof(SIM_wireslot_t));
	assert(graph->channels && graph->wires && graph->wireslots);
	uint32_t channel_count = 0;
	uint32_t wireslot_count = 0;
	for(int iw = 0; iw < graph->wires_size; ++iw)
	{
		SIM_wire_config_t *cfg = graph->config.wire_configs[iw];
		uint64_t tmpchannel_size =  cfg->ends_size;
		uint64_t tmpchannel = channel_count;
		for(int ie = 0; ie < cfg->ends_size; ++ie)
		{
			SIM_wire_config_end_t end =  cfg->ends[ie];
			int64_t entry_postion = SIM_graph_find_entry(graph, end.entry);
			if(entry_postion == -1)
			{
				//this is crude and does not allow one wire to connect to multiple ports
				assert(0 && "cannot miss entry found in wire end");
			}

			graph->channels[channel_count++] = SIM_channel_init(entry_postion, end.port);
		}


		graph->wires[iw] = SIM_wire_init(tmpchannel, tmpchannel_size, wireslot_count, cfg->delay);
		wireslot_count += cfg->delay;
	}

	graph->flags.set = true;
	return true;
}
