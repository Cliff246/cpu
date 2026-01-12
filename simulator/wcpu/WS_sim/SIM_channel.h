#ifndef __WS_SIM_CHANNEL_HEADER__
#define __WS_SIM_CHANNEL_HEADER__

#include <stdint.h>
#include <stdbool.h>

#include "SIM_packet.h"
#include "OBJ_message.h"
#include "OBJ_bundle.h"

//control unit for each end port
//designed to hold and manage merging and more
typedef struct WS_SIM_channel
{
	//device id
	uint16_t oid;
	//port id
	uint8_t pid;

	uint16_t state;
}SIM_channel_t;


SIM_channel_t SIM_channel_init(uint32_t oid, uint32_t pid);

void SIM_channel_input(SIM_channel_t *chnl, SIM_packet_t pkt);
void SIM_channel_output(SIM_channel_t *chnl, OBJ_msg_t msg);
#endif