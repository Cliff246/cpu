#include "OBJ_message.h"
#include "SIM_packet.h"
#include "SIM_channel.h"
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
SIM_channel_t SIM_channel_init(uint32_t oid, uint32_t pid)
{
	SIM_channel_t channel =
	{
		.oid = oid,
		.pid = pid
	};

	return channel;

}

void SIM_channel_input(SIM_channel_t *chnl, SIM_packet_t pkt)
{
	assert(0 && "TODO INPUT");

}
void SIM_channel_output(SIM_channel_t *chnl, OBJ_msg_t msg)
{
	assert(0 && "TODO OUTPUT");
}