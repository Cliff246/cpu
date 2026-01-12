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

void SIM_graph_add_wire(SIM_graph_t *graph, SIM_wire_config_t *wire)
{
	assert(graph->flags.set != true );
	assert(wire->flags.set == true);
	graph->config.wire_configs = realloc_safe(graph->config.wire_configs, graph->config.wire_configs_size + 1, sizeof(SIM_wire_config_t *));
	graph->config.wire_configs[graph->config.wire_configs_size++] = wire;

	graph->flags.changed = true;
	graph->flags.set = false;
}


bool SIM_graph_set(SIM_graph_t *graph)
{

	assert(graph);
	assert(graph->flags.set == false);


	assert(graph->objects_size >= 2);
	assert(graph->config.wire_configs_size >= 1);
	assert(graph->objects_size);


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
			int64_t entry_postion = end.oid;
			if(entry_postion == -1)
			{
				//this is crude and does not allow one wire to connect to multiple ports
				assert(0 && "cannot miss entry found in wire end");
			}

			graph->channels[channel_count++] = SIM_channel_init(entry_postion, end.cid);
		}


		graph->wires[iw] = SIM_wire_init(tmpchannel, tmpchannel_size, wireslot_count, cfg->delay);
		wireslot_count += cfg->delay;
	}

	graph->flags.set = true;
	return true;
}


void SIM_graph_update(SIM_graph_t *graph)
{
	const uint16_t objsize = graph->objects_size;
	const uint32_t wiresize = graph->wires_size;
	const uint32_t chnlsize = graph->channels_size;
	const uint32_t slotsize = graph->wireslots_size;

	//iwr = iterator wire read
	for(uint32_t iwr = 0; iwr < wiresize; ++iwr)
	{
		//just the wire copy
		const SIM_wire_t wire =  graph->wires[iwr];

		assert(wire.slot_start + wire.slot_length < slotsize);
		assert(wire.channel_start + wire.channel_length < chnlsize);
		assert(wire.cur_scroll < wire.slot_length);
		const SIM_packet_t *pkt =  graph->wireslots[wire.slot_start + wire.cur_scroll].packet;


		//the current wire target
		const uint8_t target = wire.tchnl;
		assert(target < wire.channel_length && "target must be less than channel length");
		//get the channel target index.
		const uint32_t target_index = target + wire.channel_start;
		assert(target_index < graph->channels_size && "channel out of bound");


		//channel from target
		const SIM_channel_t channel = graph->channels[target_index];


		//channel object id
		const uint16_t object_index = channel.oid;

		//object from object index
		SIM_object_t *obj = &graph->objects[object_index];



		//process if the object is ready


		//TODO this is not done yet

		if(wire.cur_len == 0)
		{

			//the wire is empty of any content
		}


		//scroll the packet up

		//port needs to check state off


	}





	//read loop
	//ior = iterator object read
	for(uint16_t ior = 0; ior < objsize; ++ior)
	{
		SIM_object_t *obj = &graph->objects[ior];

		OBJ_hnd_t *hnd = obj->obj_hnd;
		OBJ_ctx_t *ctx = obj->ctx;

		const uint8_t pc = obj->chnl_length;
		//ports ready count
		const uint8_t prc = obj->chnl_ready_count;
		assert(prc <= pc && "ports ready can never exceed ports count");
		//iterator ports active
		for(uint8_t ica = 0; ica < prc; ++ica)
		{
			//ready index
			const uint8_t ri = obj->chnl_ready[ica];
			//we now got all the ports ready

			const SIM_channel_t chnl = graph->channels[obj->chnl_start + ri];

			//TODO execute the port and yk... shit

		}
		//set the ports ready to zero, read from all of them
		obj->chnl_ready_count = 0;
		//there needs to be a bundle of the maximum amount of ports in
		OBJ_bnd_t *bnd = NULL;
		assert(bnd != NULL);
		obj->pipeline.read(hnd, ctx, bnd);

		//clean the bundle...
	}

	//iou = iterator object update
	//update loop
	for(uint16_t iou = 0; iou < objsize; ++iou)
	{
		SIM_object_t *obj = &graph->objects[iou];

		OBJ_hnd_t *hnd = obj->obj_hnd;
		OBJ_locals_t *locals = &obj->locals;
		OBJ_ctx_t *ctx = obj->ctx;
		obj->pipeline.update(hnd, ctx ,locals);
	}
	//write loop
	//iow = iterator object write

	for(uint16_t iow = 0; iow < objsize; ++iow)
	{

		SIM_object_t *obj = &graph->objects[iow];

		OBJ_hnd_t *hnd = obj->obj_hnd;
		OBJ_locals_t *locals = &obj->locals;
		OBJ_ctx_t *ctx = obj->ctx;

		//bundle should be new
		OBJ_bnd_t *bnd = NULL;
		assert(bnd != NULL);
		obj->pipeline.write(hnd, ctx, bnd);
		assert(bnd->size < bnd->used);
		const uint8_t bndused = bnd->used;
		//ibm = iterator bundle message
		for(uint32_t ibm = 0; ibm < bndused; ++ibm)
		{
			OBJ_msg_t msg = bnd->msgs[ibm];
			SIM_channel_t *chnl = &graph->channels[msg.pid];
			const uint8_t state = chnl->state;




			//might need to be buffered
			SIM_channel_output(chnl, msg);

		}

		//free the bundle down after


		
	}


	//write output and shit
	for(uint32_t iww = 0; iww < wiresize; ++iww)
	{
		const SIM_wire_t wire =  graph->wires[iww];
		graph->wires[iww].cur_scroll = (wire.cur_scroll == wire.slot_length)? 0 : wire.cur_scroll + 1;

	}
}