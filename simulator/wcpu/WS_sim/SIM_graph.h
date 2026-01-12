#ifndef __WS_SIM_GRAPH_HEADER__
#define __WS_SIM_GRAPH_HEADER__

#include <stdint.h>
#include <stdbool.h>

#include "SIM_object.h"
#include "SIM_wire.h"
#include "SIM_wireslot.h"
#include "SIM_channel.h"

typedef struct WS_SIM_graph
{



	uint16_t objects_size;
	SIM_object_t *objects;


	//wire_size, total number of unique wires. each wire can connect to a channel
	//each channel connects to a port
	uint32_t wires_size;
	SIM_wire_t *wires;

	//each channel is a entry index and port index for each wire in the graph
	uint32_t channels_size;
	SIM_channel_t *channels;


	//Wireslots in a straight array of the total size of wire slots
	uint32_t wireslots_size;
	SIM_wireslot_t *wireslots;

	struct
	{
		bool set;
		bool changed;
	}flags;

	struct
	{
		//uses entreies_size as length
		size_t wire_configs_size;

		SIM_wire_config_t **wire_configs;
		//should contain an alloca
	}config;

}SIM_graph_t;




SIM_graph_t *SIM_graph_init(void);
//basioc template for wire
SIM_graph_t *SIM_graph_init_template(void);

//void SIM_graph_add_object(SIM_graph_t *graph, SIM_object_t *object);
void SIM_graph_add_wire(SIM_graph_t *graph, SIM_wire_config_t *wire);

bool SIM_graph_set(SIM_graph_t *graph);

void SIM_graph_update(SIM_graph_t *graph);

#endif
