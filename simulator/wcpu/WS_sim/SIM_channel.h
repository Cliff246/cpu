#ifndef __WS_SIM_CHANNEL_HEADER__
#define __WS_SIM_CHANNEL_HEADER__


#include "SIM_packet.h"
#include "OBJ_message.h"
#include "OBJ_bundle.h"
#include "SIM_routetable.h"


#include <stdint.h>
#include <stdbool.h>

#define SIM_CHANNEL_STACK

//control unit for each end port
//designed to hold and manage merging and more
typedef struct WS_SIM_channel
{
	//device id
	uint16_t oid;
	//port id
	uint8_t cid;

	uint16_t state;
}SIM_channel_t;


SIM_channel_t SIM_channel_init(uint32_t oid, uint32_t cid);


bool SIM_channel_can_take(SIM_channel_t *chnl);
void SIM_channel_read(SIM_channel_t *chnl, SIM_packet_t *pkt);
OBJ_msg_t SIM_channel_take_input(SIM_channel_t *chnl);
void SIM_channel_take_output(SIM_channel_t *chnl, OBJ_msg_t msg);
bool SIM_channel_can_output(SIM_channel_t *chnl);
SIM_packet_t *SIM_channel_write(SIM_channel_t *chnl);

#endif