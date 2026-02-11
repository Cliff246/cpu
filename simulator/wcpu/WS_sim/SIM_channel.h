#ifndef __WS_SIM_CHANNEL_HEADER__
#define __WS_SIM_CHANNEL_HEADER__


#include "OBJ_message.h"
#include "OBJ_bundle.h"
#include "SIM_packet.h"
#include "SIM_commons.h"

#include <stdint.h>
#include <stdbool.h>

#define SIM_CHANNEL_STACK

typedef struct WS_SIM_wire SIM_wire_t;
typedef struct WS_SIM_device SIM_device_t;

//control unit for each end port
//designed to hold and manage merging and more
typedef struct WS_SIM_channel
{
	SIM_device_t *device;
	SIM_wire_t *wire;
	SIM_chnlid_t id;

	bool output;
	bool has_packet;



}SIM_channel_t;

void SIM_channel_init(SIM_channel_t *channel, uint32_t wcid, uint32_t cid, uint32_t wid);


bool SIM_channel_get_has_packet(SIM_channel_t *channel);
bool SIM_channel_get_output(SIM_channel_t *channel);

void SIM_channel_print(SIM_channel_t *channel);

#endif