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
	//boolean for whether it has an output
	bool output;
	bool changed;
	//cringe
	uint64_t address;
	SIM_packet_t packet;
}SIM_channel_t;

void SIM_channel_init(SIM_channel_t *channel, uint32_t oid, uint32_t cid);


#endif