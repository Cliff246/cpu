#include "RUN_graph.h"
#include "RUN_addrtbl.h"
#include "RUN_idmap.h"
#include "RUN_pools.h"
#include "SCENE_scene.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>


//-------------------------------------------------------
//
//	ALLOC scene
//
//-------------------------------------------------------


bool RUN_alloc_graph_idmap(RUN_graph_t *graph)
{
	const SCENE_scene_t *scene = graph->scene;

	RUN_alloc_idmap(&graph->idmap ,scene->devices_count);


	return true;

}

bool RUN_alloc_graph_pool(RUN_graph_t *graph)
{
	const SCENE_scene_t *scene = graph->scene;

	RUN_alloc_pool(&graph->pool);

	return true;

}

bool  RUN_alloc_graph_addrtbl(RUN_graph_t *graph)
{
	const SCENE_scene_t *scene = graph->scene;
	uint64_t tmp = 0;
	assert(tmp != 0);
	RUN_alloc_addrtbl(&graph->addrtbl, tmp);

	return true;

}

bool  RUN_alloc_graph(RUN_graph_t *graph)
{
	const SCENE_scene_t *scene = graph->scene;

	RUN_alloc_graph_idmap(graph);
	RUN_alloc_graph_pool(graph);
	RUN_alloc_graph_addrtbl(graph);


	/*



	printf("idmap[%lu]=%ld\n", scene->devices[1].tag, RUN_get_did_idmap(graph->idmap, scene->devices[1].tag));
	*/
	return true;

}

//-------------------------------------------------------
//
//	BUILD scene
//
//-------------------------------------------------------





bool RUN_build_graph_idmap(RUN_graph_t *graph)
{
	const SCENE_scene_t *scene = graph->scene;

	SIM_dkey_t dkeys[scene->devices_count];
	SIM_did_t did[scene->devices_count];
	for(uint64_t i = 0; i < scene->devices_count; ++i)
	{
		SCENE_device_t *dev = scene->devices[i];
		dkeys[i] = dev->dkey;
		did[i] = dev->id;
	}
	RUN_build_idmap(&graph->idmap, dkeys, did, scene->devices_count);

	return true;

}

bool RUN_build_graph_pool(RUN_graph_t *graph)
{
	const SCENE_scene_t *scene = graph->scene;

	RUN_build_pool(&graph->pool, graph->scene);

	return true;

}

bool RUN_build_graph_addrtbl(RUN_graph_t *graph)
{
	const SCENE_scene_t *scene = graph->scene;

	RUN_build_addrtbl(&graph->addrtbl, graph->scene);

	return true;

}


bool RUN_build_graph(RUN_graph_t *graph)
{

	RUN_build_graph_idmap(graph);
	RUN_build_graph_pool(graph);
	RUN_build_graph_addrtbl(graph);

	return true;
}




/*

void SCENE_graph_add_devices(SCENE_graph_t *graph, SCENE_device_t **devices, uint64_t size)
{
	graph->objects_size = size;
	graph->objects = calloc(size, sizeof(SCENE_object_t));


	for(int i = 0; i < size; ++i)
	{
		graph->objects[i] = SCENE_object_init(devices[i]);
	}
}



void SCENE_graph_object_update(SCENE_graph_t *graph)
{

	const SCENE_object_global_t objects_size = graph->objects_size;

	for(uint32_t i = 0; i < objects_size; ++i)
	{

		SCENE_object_t *object = &graph->objects[i];

		SCENE_object_update(object);

	}
}

void SCENE_graph_object_read(SCENE_graph_t *graph)
{
	const SCENE_object_global_t objects_size = graph->objects_size;

	for(uint32_t i = 0; i < objects_size; ++i)
	{

		SCENE_object_t *object = &graph->objects[i];
		assert(object);
		SCENE_port_t *port = object->port;
		assert(port);

		OBJ_bundle_t bnd = {0};
		assert(0 && "TODO");

		bool produced = SCENE_port_produce_bundle(graph, port,&bnd);

		if(!produced)
		{
			assert(0 && "production of bundle and reading of channels failed");
		}

		SCENE_object_read(object, &bnd);




	}
}

void SCENE_graph_object_write(SCENE_graph_t *graph)
{
	const SCENE_object_global_t objects_size = graph->objects_size;

	for(uint32_t i = 0; i < objects_size; ++i)
	{

		SCENE_object_t *object = &graph->objects[i];
		assert(object);

		SCENE_port_t *port = object->port;
		assert(port);
		OBJ_bundle_t bnd = {0};



		assert(0 && "TODO");

		SCENE_object_write(object, &bnd);
		//todo gather bundle information send to port
		//collect and distrubute out
		bool recieved = SCENE_port_recieve_bundle(graph, port, &bnd);

		if(!recieved)
		{
			assert(0 && "recieving of bundle and writing of channels failed");
		}


	}
}

void SCENE_graph_wire_read(SCENE_graph_t *graph)
{
	const uint32_t wire_size = graph->wires_size;

	for(uint32_t iwr = 0; iwr < wire_size; ++iwr)
	{
		SCENE_wire_t *wire = &graph->wires[iwr];
		if(wire->transfering)
		{
			SCENE_wire_channel_t output =  SCENE_wire_bus_get_output(wire);
			SCENE_channel_global_t global_output =  SCENE_wire_channel_convert(wire, output);
			SCENE_channel_t *channel = SCENE_graph_get_channel(graph, global_output);
			SCENE_transfer_global_t global_transfer = SCENE_wire_get_current_transfer_global(wire);

			//wire read from the graph and get packet via a copy

			SCENE_transfer_t *transfer = SCENE_graph_get_transfer(graph, global_transfer);
			SCENE_packet_t packet = {0};

			bool transfer_success = SCENE_transfer_read(transfer, &packet);
			if(!transfer_success)
			{
				assert(0 && "TODO transfer failed");
			}


			bool channel_set_success = SCENE_channel_set_packet(channel, packet);

			if(!channel_set_success)
			{
				assert(0 && "TODO set failed");

			}



		}
	}

}

void SCENE_graph_wire_write(SCENE_graph_t *graph)
{
	const uint32_t wire_size = graph->wires_size;

	for(uint32_t iww = 0; iww < wire_size; ++iww)
	{
		SCENE_wire_t *wire = &graph->wires[iww];
		SCENE_wire_channel_t output =  SCENE_wire_bus_get_output(wire);
		SCENE_channel_global_t global_output =  SCENE_wire_channel_convert(wire, output);
		SCENE_channel_t *channel = SCENE_graph_get_channel(graph, global_output);

		SCENE_transfer_global_t global_transfer = SCENE_wire_get_current_transfer_global(wire);
		SCENE_transfer_t *transfer = SCENE_graph_get_transfer(graph, global_transfer);

		SCENE_packet_t packet = {0};


		bool channel_get_success = SCENE_channel_get_packet(channel, &packet);
		if(!channel_get_success)
		{
			assert(0 && "TODO get failed");
		}

		bool transfer_success = SCENE_transfer_send(transfer, packet);
		if(!transfer_success)
		{
			assert(0 && "TODO transfer send failed");
		}
		SCENE_wire_update_scroll(wire);
	}
}




void SCENE_graph_update(SCENE_graph_t *graph)
{

	assert(graph->flags.changed == false);
	SCENE_graph_wire_read(graph);
	//a waste for now but i am so lazy
	SCENE_graph_object_read(graph);
	SCENE_graph_object_update(graph);
	SCENE_graph_object_write(graph);
	//another waste for now but i am soooooooo lazy
	SCENE_graph_wire_write(graph);
}

SCENE_transfer_t *SCENE_graph_get_transfer(SCENE_graph_t *graph, SCENE_transfer_global_t global)
{
	assert(global);
	assert(global < graph->transfer_size);
	assert(global >= 0);
	return &graph->transfers[global];
}
*/
