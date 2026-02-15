#include "RUN_graph.h"
#include "SIM_stage.h"
#include <assert.h>
#include <stdlib.h>

void RUN_generate_graph_idmap(RUN_graph_t *graph, SIM_stage_t *stage)
{
	SIM_dtag_t dtags[stage->devices_count];
	SIM_did_t did[stage->devices_count];
	for(uint64_t i = 0; i < stage->devices_count; ++i)
	{
		SIM_device_t *dev = &stage->devices[i];
		dtags[i] = dev->tag;
		did[i] = dev->id;
	}
	RUN_init_idmap(&graph->idmap ,dtags, did, stage->devices_count);

}

void RUN_generate_graph_pool(RUN_graph_t *graph, SIM_stage_t *stage)
{

}

RUN_graph_t *RUN_alloc_graph(SIM_stage_t *stage)
{
	RUN_graph_t *graph = calloc(1, sizeof(RUN_graph_t));
	assert(graph);


	RUN_generate_graph_idmap(graph, stage);




	/*



	printf("idmap[%lu]=%ld\n", stage->devices[1].tag, RUN_get_did_idmap(graph->idmap, stage->devices[1].tag));
	*/
}

bool RUN_build_graph(RUN_graph_t *graph)
{

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