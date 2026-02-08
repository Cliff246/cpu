#ifndef __WS_SIM_WIRE_HEADER__
#define __WS_SIM_WIRE_HEADER__

#include "OBJ_constants.h"
#include "SIM_commons.h"
#include "SIM_port.h"
#include "SIM_router.h"
#include "SIM_wirecfg.h"
#include "SIM_wirering.h"
#include <SIM_channel.h>
#include <SIM_bus.h>

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/types.h>




//contains wire information, number of slots, base state, etc
typedef struct WS_SIM_wire
{
	SIM_router_t router;
	SIM_wirering_t wirering;
	SIM_packetbuffer_t *buffer;
	uint64_t id;
}SIM_wire_t;


bool SIM_init_wire(SIM_wire_t *wire, SIM_wirecfg_t *cfg);
void SIM_print_wire(SIM_wire_t *wire);

//static_assert(OBJ_MAX_CHANNELS == sizeof(uint32_t) * 8, "object max channels must equal 32");


/*
typedef struct WS_SIM_graph SIM_graph_t;

SIM_wire_t SIM_wire_init(uint32_t channel_start, uint32_t channel_length, uint32_t transfer_start, uint32_t transfer_length, uint16_t index);
SIM_channel_global_t SIM_wire_channel_has(SIM_graph_t *graph, SIM_wire_t *wire);

bool SIM_wire_bus_used_channel_roundrobin(SIM_wire_t *wire, SIM_wire_channel_t local);
void SIM_wire_bus_set_used_channel_roundrobin(SIM_wire_t *wire, SIM_wire_channel_t local);
void SIM_wire_bus_clear_used_channel_roundrobin(SIM_wire_t *wire, SIM_wire_channel_t local);
SIM_channel_global_t SIM_wire_channel_convert(SIM_wire_t *wire, SIM_wire_channel_t local);
void SIM_wire_update_scroll(SIM_wire_t *wire);
SIM_transfer_global_t SIM_wire_get_current_transfer_global(SIM_wire_t *wire);
SIM_wire_channel_t SIM_wire_bus_get_input(SIM_wire_t *wire);
SIM_wire_channel_t SIM_wire_bus_get_output(SIM_wire_t *wire);
void SIM_wire_bus_add_channel_roundrobin(SIM_wire_t *wire, SIM_wire_channel_t local);
SIM_wire_channel_t SIM_wire_bus_get_channel_roundrobin(SIM_wire_t *wire);
bool SIM_wire_bus_dequeue_roundrobin(SIM_graph_t *graph, SIM_wire_t *wire);

void SIM_wire_bus_complete_channel_roundrobin(SIM_wire_t *wire);
bool SIM_wire_bus_has_channel_roundrobin(SIM_wire_t *wire);

*/

#endif
