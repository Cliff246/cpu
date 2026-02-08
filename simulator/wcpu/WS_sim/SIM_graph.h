#ifndef __WS_SIM_GRAPH_HEADER__
#define __WS_SIM_GRAPH_HEADER__

#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

#include "SIM_commons.h"
#include "SIM_device.h"
#include "SIM_port.h"
#include "SIM_bus.h"
#include "SIM_wire.h"
#include "SIM_transfer.h"
#include "SIM_channel.h"
#include "SIM_routemap.h"
#include "SIM_context.h"



typedef struct WS_SIM_graph
{

	SIM_context_t *context;

	uint32_t channels_count;
	uint32_t devices_count;
	uint32_t wires_count;

	SIM_channel_t *channels;

	SIM_device_t *devices;

	SIM_wire_t *wires;


}SIM_graph_t;




SIM_graph_t *SIM_graph_init(SIM_context_t *context);


/*

SIM_graph_t *SIM_graph_init(SIM_context_t *context);

void SIM_graph_add_devices(SIM_graph_t *graph, SIM_device_t **devices, uint64_t size);

SIM_channel_t *SIM_graph_get_channel(SIM_graph_t *graph, SIM_channel_global_t global);
SIM_transfer_t *SIM_graph_get_transfer(SIM_graph_t *graph, SIM_transfer_global_t global);
SIM_wire_t *SIM_graph_get_wire(SIM_graph_t *graph, SIM_wire_global_t wire);
//set graph changed function
//sets graphs flags to be modified
void SIM_graph_set_changed(SIM_graph_t *graph);

void SIM_graph_wire_read(SIM_graph_t *graph);
void SIM_graph_wire_write(SIM_graph_t *graph);

void SIM_graph_object_update(SIM_graph_t *graph);
void SIM_graph_object_read(SIM_graph_t *graph);
void SIM_graph_object_write(SIM_graph_t *graph);

//reuturns 16 bit int -1 if invalid and 8 bit uint if valid
int16_t SIM_graph_find_channel_open_routetable(SIM_graph_t *graph, SIM_object_global_t oid, uint8_t route);

//void SIM_graph_add_object(SIM_graph_t *graph, SIM_object_t *object);

//



bool SIM_graph_set(SIM_graph_t *graph);


//main graph update loop
void SIM_graph_update(SIM_graph_t *graph);

*/
#endif
