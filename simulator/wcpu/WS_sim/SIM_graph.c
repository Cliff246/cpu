#include "SIM_graph.h"
#include "OBJ_bundle.h"
#include "SIM_channel.h"
#include "SIM_commons.h"
#include "SIM_context.h"
#include "SIM_device.h"
#include "SIM_devicelist.h"
#include "SIM_mailbox.h"
#include "SIM_packet.h"
#include "SIM_port.h"
#include "SIM_transfer.h"
#include "SIM_wire.h"
#include "SIM_wireconfig.h"
#include "commons.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <sys/types.h>

void SIM_graph_fill_wires(SIM_graph_t *graph)
{


	SIM_context_t *context = graph->context;
	const uint64_t size = SIM_get_devicelist_size(context->devicelist);
	for(uint64_t i = 0; i < context->wireconfigs_size; ++i)
	{
		//printf("%d\n", i);
		SIM_wireconfig_t *wireconfig = &context->wireconfigs[i];
		SIM_channel_t *channels[size];
		uint64_t j = 0;

		for(uint64_t k = 0; k < size; ++k)
		{
			SIM_device_t *device = SIM_get_device_devicelist(context->devicelist, k);
			SIM_channel_t *channel = SIM_device_get_channel_by_wireid(device, wireconfig->id);
			channels[j++] = channel;
		}
		//printf("J:%d\n", j);
		SIM_wire_t *wire = SIM_init_wire(channels, j, *wireconfig);
		SIM_print_wire(wire);
	}



}

SIM_graph_t *SIM_graph_init(SIM_context_t *context)
{

	SIM_graph_t *graph = calloc(1, sizeof(SIM_graph_t));
	assert(graph);
	graph->context = context;

	const uint64_t size = SIM_get_devicelist_size(graph->context->devicelist);
	SIM_graph_fill_wires(graph);

	return graph;
}

/*

void SIM_graph_add_devices(SIM_graph_t *graph, SIM_device_t **devices, uint64_t size)
{
	graph->objects_size = size;
	graph->objects = calloc(size, sizeof(SIM_object_t));


	for(int i = 0; i < size; ++i)
	{
		graph->objects[i] = SIM_object_init(devices[i]);
	}
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
		assert(object);
		SIM_port_t *port = object->port;
		assert(port);

		OBJ_bundle_t bnd = {0};
		assert(0 && "TODO");

		bool produced = SIM_port_produce_bundle(graph, port,&bnd);

		if(!produced)
		{
			assert(0 && "production of bundle and reading of channels failed");
		}

		SIM_object_read(object, &bnd);




	}
}

void SIM_graph_object_write(SIM_graph_t *graph)
{
	const SIM_object_global_t objects_size = graph->objects_size;

	for(uint32_t i = 0; i < objects_size; ++i)
	{

		SIM_object_t *object = &graph->objects[i];
		assert(object);

		SIM_port_t *port = object->port;
		assert(port);
		OBJ_bundle_t bnd = {0};



		assert(0 && "TODO");

		SIM_object_write(object, &bnd);
		//todo gather bundle information send to port
		//collect and distrubute out
		bool recieved = SIM_port_recieve_bundle(graph, port, &bnd);

		if(!recieved)
		{
			assert(0 && "recieving of bundle and writing of channels failed");
		}


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

			//wire read from the graph and get packet via a copy

			SIM_transfer_t *transfer = SIM_graph_get_transfer(graph, global_transfer);
			SIM_packet_t packet = {0};

			bool transfer_success = SIM_transfer_read(transfer, &packet);
			if(!transfer_success)
			{
				assert(0 && "TODO transfer failed");
			}


			bool channel_set_success = SIM_channel_set_packet(channel, packet);

			if(!channel_set_success)
			{
				assert(0 && "TODO set failed");

			}



		}
	}

}

void SIM_graph_wire_write(SIM_graph_t *graph)
{
	const uint32_t wire_size = graph->wires_size;

	for(uint32_t iww = 0; iww < wire_size; ++iww)
	{
		SIM_wire_t *wire = &graph->wires[iww];
		SIM_wire_channel_t output =  SIM_wire_bus_get_output(wire);
		SIM_channel_global_t global_output =  SIM_wire_channel_convert(wire, output);
		SIM_channel_t *channel = SIM_graph_get_channel(graph, global_output);

		SIM_transfer_global_t global_transfer = SIM_wire_get_current_transfer_global(wire);
		SIM_transfer_t *transfer = SIM_graph_get_transfer(graph, global_transfer);

		SIM_packet_t packet = {0};


		bool channel_get_success = SIM_channel_get_packet(channel, &packet);
		if(!channel_get_success)
		{
			assert(0 && "TODO get failed");
		}

		bool transfer_success = SIM_transfer_send(transfer, packet);
		if(!transfer_success)
		{
			assert(0 && "TODO transfer send failed");
		}
		SIM_wire_update_scroll(wire);
	}
}




void SIM_graph_update(SIM_graph_t *graph)
{

	assert(graph->flags.changed == false);
	SIM_graph_wire_read(graph);
	//a waste for now but i am so lazy
	SIM_graph_object_read(graph);
	SIM_graph_object_update(graph);
	SIM_graph_object_write(graph);
	//another waste for now but i am soooooooo lazy
	SIM_graph_wire_write(graph);
}

SIM_transfer_t *SIM_graph_get_transfer(SIM_graph_t *graph, SIM_transfer_global_t global)
{
	assert(global);
	assert(global < graph->transfer_size);
	assert(global >= 0);
	return &graph->transfers[global];
}
*/