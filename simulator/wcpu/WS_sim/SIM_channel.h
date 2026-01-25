#ifndef __WS_SIM_CHANNEL_HEADER__
#define __WS_SIM_CHANNEL_HEADER__


#include "OBJ_message.h"
#include "OBJ_bundle.h"
#include "SIM_packet.h"
#include "SIM_routemap.h"
#include "SIM_commons.h"

#include <stdint.h>
#include <stdbool.h>

#define SIM_CHANNEL_STACK




//control unit for each end port
//designed to hold and manage merging and more
typedef struct WS_SIM_channel
{
	//object id
	SIM_object_global_t oid;
	//channel id
	SIM_channel_local_t cid;
	//channel wire id
	SIM_wire_global_t wid;
	//boolean for whether it has an output
	bool output;
	bool has_packet;
	//cringe
	uint64_t address;
	SIM_packet_t packet;
}SIM_channel_t;

void SIM_channel_init(SIM_channel_t *channel, uint32_t oid, uint32_t cid, uint32_t wid);


bool SIM_channel_get_has_packet(SIM_channel_t *channel);
bool SIM_channel_get_output(SIM_channel_t *channel);

//sets a packet if there is no packet used
//returns true on success and false on packet set
//sets has_packet = true
bool SIM_channel_set_packet(SIM_channel_t *channel, SIM_packet_t packet);


//gets a packet if output is true and has_packet is true
//sets has_packet = false
bool SIM_channel_get_packet(SIM_channel_t *channel, SIM_packet_t *packet);


#endif