
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include "SIM_graph.h"
#include "SIM_commons.h"
#include "SIM_device.h"
#include "SIM_mailbox.h"
#include "SIM_port.h"
#include "SIM_vistable.h"
#include "SIM_wire.h"
#include "SIM_transfer.h"
#include "SIM_channel.h"
#include "SIM_context.h"
#include "SIM_stage.h"

SIM_graph_t *SIM_init_graph(SIM_stage_t *stage)
{
	SIM_graph_t *graph =calloc(1, sizeof(SIM_graph_t));
	assert(graph);


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