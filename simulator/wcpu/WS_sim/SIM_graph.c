#include "SIM_graph.h"
#include "OBJ_constants.h"
#include "OBJ_message.h"
#include "SIM_channel.h"
#include "SIM_port.h"
#include "SIM_wireslot.h"
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

	graph->wireslots_size = wireslot_size;
	graph->wires_size = graph->config.wire_configs_size;
	graph->channels_size = channel_size;
	graph->channels = calloc(graph->channels_size, sizeof(SIM_channel_t));
	assert(graph->channels && "graph channels alloc");
	graph->wires = calloc(graph->wires_size, sizeof(SIM_wire_t));
	assert(graph->wires && "graph wires alloc");
	graph->wireslots = calloc(graph->wireslots_size, sizeof(SIM_wireslot_t));
	assert(graph->wireslots && "graph wireslots alloc");
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
int16_t SIM_graph_find_channel_open_routetable(SIM_graph_t *graph, uint16_t oid, uint8_t route)
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
		const uint32_t port_chnl_id = port->channels[route_chnl];
		SIM_channel_t *channel = &graph->channels[port_chnl_id];



		assert(0);
		{
			return route_chnl;
		}
	}
	return -1;
}


void SIM_graph_update(SIM_graph_t *graph)
{

	assert(graph->flags.changed == false);

	const uint16_t objsize = graph->objects_size;
	const uint32_t wiresize = graph->wires_size;
	const uint32_t chnlsize = graph->channels_size;
	const uint32_t slotsize = graph->wireslots_size;

	//iwr = iterator wire read
	for(uint32_t iwr = 0; iwr < wiresize; ++iwr)
	{
		//tgt:import

		//just the wire copy
		SIM_wire_t *wire =  &graph->wires[iwr];

		assert(wire->slot_start + wire->slot_length < slotsize);
		assert(wire->channel_start + wire->channel_length < chnlsize);
		assert(wire->cur_scroll < wire->slot_length);
		SIM_wireslot_t *slot =  &graph->wireslots[wire->slot_start + wire->cur_scroll];
		if(slot->used == false)
		{
			continue;
		}
		OBJ_msg_t *msg = &slot->msg;

		//the current wire target
		const uint8_t target = wire->tgt_chnl;
		assert(target < wire->channel_length && "target must be less than channel length");
		//get the channel target index.
		const uint32_t target_index = target + wire->channel_start;
		assert(target_index < graph->channels_size && "channel out of bound");

		//channel from target
		SIM_channel_t *tgt_chnl = &graph->channels[target_index];
		if(tgt_chnl->has == false)
		{
			memcpy(&tgt_chnl->msg, msg, sizeof(OBJ_msg_t));
			slot->used = false;
			wire->cur_len--;
			tgt_chnl->has = true;
			if(msg->index == msg->size)
			{
				wire->tgt_done = true;
			}
		}
		else
		{
			assert(0);
		}
	}
	//read loop
	//ior = iterator object read
	for(uint16_t ior = 0; ior < objsize; ++ior)
	{
		SIM_object_t *obj = &graph->objects[ior];

		OBJ_hnd_t *hnd = obj->obj_hnd;
		OBJ_ctx_t *ctx = obj->ctx;

		//there needs to be a bundle of the maximum amount of ports in
		OBJ_bundle_t bnd = {0};
		SIM_port_t *port = &graph->ports[obj->port_index];
		SIM_port_channel_active_recieve(graph, port);
		SIM_port_channel_recieve(graph, port);

		if(!SIM_port_can_read(graph, port))
		{
			assert(0);
		}

		SIM_port_bundle_write(port, &bnd);

		assert(obj->pipeline.read != NULL);
		obj->pipeline.read(hnd, ctx, &bnd);
	}

	//iou = iterator object update
	//update loop
	for(uint16_t iou = 0; iou < objsize; ++iou)
	{
		SIM_object_t *obj = &graph->objects[iou];

		OBJ_hnd_t *hnd = obj->obj_hnd;
		//locals are undone... need to be fixed
		OBJ_locals_t *locals = obj->locals;
		OBJ_ctx_t *ctx = obj->ctx;
		obj->pipeline.update(hnd, ctx ,locals);
	}
	//write loop
	//iow = iterator object write

	for(uint16_t iow = 0; iow < objsize; ++iow)
	{

		SIM_object_t *obj = &graph->objects[iow];

		OBJ_hnd_t *hnd = obj->obj_hnd;
		OBJ_ctx_t *ctx = obj->ctx;

		//bundle should be new

		SIM_port_t *port = &graph->ports[obj->port_index];

		if(!SIM_port_can_write(graph, port))
		{
			assert(0);
		}

		OBJ_bundle_t bnd = {0};
		//OBJECT write function
		assert(obj->pipeline.write != NULL);
		obj->pipeline.write(hnd, ctx, &bnd);
		SIM_port_bundle_read(port, &bnd);
		SIM_port_channel_release(graph, port);
		SIM_port_channel_active_release(graph, port);

		assert(0 && "TODO free the bundle or clean it");
	}


	//write output and shit
	for(uint32_t iww = 0; iww < wiresize; ++iww)
	{
		//src:export
		SIM_wire_t *wire =  &graph->wires[iww];

		const uint32_t chnl_start = wire->channel_start;
		const uint32_t chnl_length = wire->channel_length;
		const uint8_t src_chnl_offset = wire->src_chnl;
		const uint8_t tgt_chnl_offset = wire->tgt_chnl;
		assert(src_chnl_offset < chnl_length && tgt_chnl_offset < chnl_length);

		const uint32_t src_chnl_idx = chnl_start + src_chnl_offset;
		const uint32_t tgt_chnl_idx = chnl_start + tgt_chnl_offset;

		SIM_channel_t *src_chnl = &graph->channels[src_chnl_idx];

		const uint32_t t_cur_scroll = wire->cur_scroll;

		SIM_wireslot_t *slot = &graph->wireslots[t_cur_scroll];

		if(slot->used == true)
		{
			assert(0);
		}
		if(src_chnl->has == true)
		{
			memcpy(&slot->msg, &src_chnl->msg, sizeof(OBJ_msg_t));
			slot->used = true;
			wire->cur_len++;
			src_chnl->has = false;
				//set target
		}
		else
		{
			wire->src_done = true;
			//
		}
		// Arbitration rule (UNDEFINED):
		// Select next src/tgt channel pair for this wire.
		// Must ensure progress without starvation.
		// Must not violate channel ownership invariants.

		if(wire->tgt_done && wire->src_done)
		{
			//arbitrate new tgt and src
			assert(0);
		}

		wire->cur_scroll = (t_cur_scroll == wire->slot_length)? 0 : t_cur_scroll + 1;

	}
}
