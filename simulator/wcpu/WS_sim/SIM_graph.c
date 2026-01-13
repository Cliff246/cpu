#include "SIM_graph.h"
#include "commons.h"
#include "SIM_routetable.h"

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
	//TODO ROUTINGTABLE
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
	assert(graph->routetable);
	graph->flags.set = true;
	return true;
}
//assumes an open routetable
//returns -1 on failure and on success returns an index in an object channels
int16_t SIM_graph_find_channel_open_routetable(SIM_graph_t *graph, uint16_t oid, uint8_t route)
{
	SIM_object_t *obj = &graph->objects[oid];
	const uint16_t rtid = obj->routetable_id;
	SIM_routetable_t *rt = &graph->routetable[rtid];

	SIM_route_t *r = &rt->table[route];
	const uint32_t obj_chnl_start = obj->chnl_start;

	//iterator overlap
	for(uint16_t io = 0; io < r->overlap_length; ++io)
	{
		//route channel
		const uint8_t rchnl = rt->overlap[r->overlap_start + io];
		SIM_channel_t *channel = &graph->channels[obj_chnl_start + rchnl];



		if(SIM_channel_can_take(channel))
		{
			return rchnl;
		}
	}
	return -1;
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
		const SIM_wire_t *wire =  &graph->wires[iwr];

		assert(wire->slot_start + wire->slot_length < slotsize);
		assert(wire->channel_start + wire->channel_length < chnlsize);
		assert(wire->cur_scroll < wire->slot_length);
		const SIM_packet_t *pkt =  graph->wireslots[wire->slot_start + wire->cur_scroll].packet;
		if(pkt == NULL)
		{
			continue;
		}

		//the current wire target
		const uint8_t target = wire->tgt_chnl;
		assert(target < wire->channel_length && "target must be less than channel length");
		//get the channel target index.
		const uint32_t target_index = target + wire->channel_start;
		assert(target_index < graph->channels_size && "channel out of bound");


		//channel from target
		const SIM_channel_t *channel = &graph->channels[target_index];


		//channel object id
		const uint16_t object_index = channel->oid;
		assert(object_index < graph->objects_size);
		//object from object index
		SIM_object_t *obj = &graph->objects[object_index];


		SIM_channel_read(channel, pkt);
		//process if the object is ready


		//TODO this is not done yet

		if(wire->cur_len == 0)
		{

			obj->chnl_ready[obj->chnl_ready_length++] = target;
			//the wire is empty of any content
		}


		//scroll the packet up

		//channel needs to check state off


	}





	//read loop
	//ior = iterator object read
	for(uint16_t ior = 0; ior < objsize; ++ior)
	{
		SIM_object_t *obj = &graph->objects[ior];

		OBJ_hnd_t *hnd = obj->obj_hnd;
		OBJ_ctx_t *ctx = obj->ctx;

		//there needs to be a bundle of the maximum amount of ports in
		OBJ_bnd_t *bnd = NULL;
		assert(bnd != NULL);

		const uint8_t cc = obj->chnl_length;
		//ports ready count
		const uint8_t crc = obj->chnl_ready_length;
		assert(crc <= cc && "ports ready can never exceed ports count");



		//iterator channels active
		for(uint8_t ica = 0; ica < crc; ++ica)
		{
			//ready index
			const uint8_t ri = obj->chnl_ready[ica];
			//we now got all the ports ready

			const SIM_channel_t *chnl = &graph->channels[obj->chnl_start + ri];


			OBJ_msg_t msg = SIM_channel_take_input(chnl);


			bnd->msgs[bnd->used++] = msg;

			//TODO execute the port and yk... shit

		}

		//set the ports ready to zero, read from all of them
		obj->chnl_ready_length = 0;
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
		OBJ_ctx_t *ctx = obj->ctx;

		//bundle should be new
		OBJ_bnd_t *bnd = NULL;
		assert(bnd != NULL);
		obj->pipeline.write(hnd, ctx, bnd);
		assert(bnd->size > bnd->used);
		const uint8_t bndused = bnd->used;
		//ibm = iterator bundle message
		for(uint32_t ibm = 0; ibm < bndused; ++ibm)
		{
			OBJ_msg_t *msg = &bnd->msgs[ibm];
			SIM_channel_t *chnl = NULL;

			//this is very stupid but oh well
			if(msg->target == OBJ_MSG_TARGET_CHANNEL)
			{
				const uint8_t cid = msg->tag.channel;
				chnl = &graph->channels[obj->chnl_start + cid];
				if(!SIM_channel_can_take(chnl))
				{
					assert(0);
					exit(EXIT_FAILURE);

				}

			}
			//make this better eventually
			else if(msg->target == OBJ_MSG_TARGET_ADDRESS)
			{
				//do a local address search of the routetable
				const uint64_t addr = msg->tag.address;
				const uint16_t rtid = obj->routetable_id;
				assert(rtid < graph->routingtable_size);

				SIM_routetable_t *rt = &graph->routetable[rtid];

				int32_t route_id = SIM_routetable_search(rt, addr);
				if(route_id == -1)
				{
					assert(0);
					exit(EXIT_FAILURE);
				}
				assert(route_id >= 0 && route_id < rt->rows);

				bool overlaps = SIM_routetable_overlaps(rt, route_id);
				if(!overlaps)
				{
					assert(0);
					exit(EXIT_FAILURE);
				}
				//iterator ioverlap
				int16_t open_chnl = SIM_graph_find_channel_open_routetable(graph, iow, route_id);
				if(open_chnl == -1)
				{
					assert(0 && "no open channels ");
					exit(EXIT_FAILURE);
				}
				chnl = &graph->channels[obj->chnl_start + open_chnl];
			}
			else
			{
				assert(0);
				exit(EXIT_FAILURE);
			}


			//might need to be buffered
			SIM_channel_take_output(chnl, *msg);

		}

		//free the bundle down after



	}


	//write output and shit
	for(uint32_t iww = 0; iww < wiresize; ++iww)
	{
		SIM_wire_t *wire =  &graph->wires[iww];
		const uint8_t chnl_length = wire->channel_length;
		const uint8_t src_chnl = wire->src_chnl;
		const uint32_t chnl_index = wire->channel_start + src_chnl;
		SIM_channel_t *chnl = &graph->channels[chnl_index];

		if(SIM_channel_can_output(chnl))
		{
			SIM_packet_t *pkt = SIM_channel_write(chnl);
			const uint32_t slot_start = wire->slot_start;
			const uint32_t slot_length = wire->slot_length;

			graph->wireslots[slot_start + wire->cur_scroll].packet = pkt;
			wire->cur_len++;
		}
		else if(wire->cur_len == 0)
		{
			//TODO start up a new packet
			assert(0 && "TODO");
		}
		//keep moving the packets forward

		wire->cur_scroll = (wire->cur_scroll == wire->slot_length)? 0 : wire->cur_scroll + 1;

	}
}