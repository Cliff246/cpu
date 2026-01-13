#include "OBJ_message.h"
#include "SIM_packet.h"
#include "SIM_channel.h"
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
SIM_channel_t SIM_channel_init(uint32_t oid, uint32_t cid)
{
	SIM_channel_t channel =
	{
		.oid = oid,
		.cid = cid
	};

	return channel;

}

bool SIM_channel_can_take(SIM_channel_t *chnl)
{
	assert(0 && "TODO can take more data");
}

void SIM_channel_read(SIM_channel_t *chnl, SIM_packet_t *pkt)
{
	assert(0 && "TODO TAKE INPUT");

}
OBJ_msg_t SIM_channel_take_input(SIM_channel_t *chnl)
{
	assert(0 && "TODO TAKE INPUT");

}


void SIM_channel_take_output(SIM_channel_t *chnl, OBJ_msg_t msg)
{
	assert(0 && "TODO TAKE OUTPUT");
}

bool SIM_channel_can_output(SIM_channel_t *chnl)
{
	assert(0 && "TODO CAN OUTPUT");

}

SIM_packet_t *SIM_channel_write(SIM_channel_t *chnl)
{
	assert(0 && "TODO WRITE");

}