#ifndef __WS_SIM_WIRE_HEADER__
#define __WS_SIM_WIRE_HEADER__

#include "OBJ_constants.h"
#include "SIM_commons.h"
#include "SIM_port.h"
#include <SIM_channel.h>

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct WS_SIM_wire_config_end
{
	uint16_t oid;
	uint8_t cid;
}SIM_wire_config_end_t;

SIM_wire_config_end_t SIM_wire_config_end_init(uint16_t oid, uint8_t cid);



typedef struct WS_SIM_wire_config
{

	uint32_t delay;
	//this is the maximum amount a wire can hit the end
	uint8_t ends_size;
	SIM_wire_config_end_t *ends;
	struct
	{
		bool set;
	}flags;
}SIM_wire_config_t;

SIM_wire_config_t *SIM_wire_config_init(uint32_t delay);

void SIM_wire_config_add_end(SIM_wire_config_t *wire_config, SIM_wire_config_end_t end);

bool SIM_wire_config_set(SIM_wire_config_t *wire_config);


//contains wire information, number of slots, base state, etc
typedef struct WS_SIM_wire
{

	//designed for channels, shows where the wire can end up
	uint8_t 			channel_length;
	uint8_t 			bus_rr_last;
	uint8_t 			bus_rr_length;
	SIM_wire_channel_t  transfer_channel_output;
	SIM_wire_channel_t 	transfer_channel_input;
	bool 				transfering;
	uint32_t 			channel_start;


	uint32_t 			transfer_start;
	uint32_t 			transfer_length;

	//current index of the scroll limit
	uint32_t			current_transfer_scroll;
	uint32_t 			current_transfer_length;

	//round robin queue
	SIM_wire_channel_t bus_rr[OBJ_MAX_CHANNELS];
	uint32_t bus_rr_used;


}SIM_wire_t;

static_assert(OBJ_MAX_CHANNELS == sizeof(uint32_t) * 8, "object max channels must equal 32");

typedef struct WS_SIM_graph SIM_graph_t;

SIM_wire_t SIM_wire_init(uint32_t channel_start, uint32_t channel_length, uint32_t transfer_start, uint32_t transfer_length);
SIM_channel_global_t SIM_wire_channel_has(SIM_graph_t *graph, SIM_wire_t *wire);

bool SIM_wire_bus_used_channel_roundrobin(SIM_wire_t *wire, SIM_wire_channel_t local);
void SIM_wire_bus_set_used_channel_roundrobin(SIM_wire_t *wire, SIM_wire_channel_t local);
void SIM_wire_bus_clear_used_channel_roundrobin(SIM_wire_t *wire, SIM_wire_channel_t local);
SIM_channel_global_t SIM_wire_channel_convert(SIM_wire_t *wire, SIM_wire_channel_t local);
void SIM_wire_update_scroll(SIM_wire_t *wire);

void SIM_wire_bus_add_channel_roundrobin(SIM_wire_t *wire, SIM_wire_channel_t local);
SIM_wire_channel_t SIM_wire_bus_get_channel_roundrobin(SIM_wire_t *wire);
bool SIM_wire_bus_dequeue_roundrobin(SIM_graph_t *graph, SIM_wire_t *wire);

void SIM_wire_bus_complete_channel_roundrobin(SIM_wire_t *wire);
bool SIM_wire_bus_has_channel_roundrobin(SIM_wire_t *wire);

#endif
