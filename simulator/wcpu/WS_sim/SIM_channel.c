#include "OBJ_message.h"
#include "SIM_channel.h"
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>


void SIM_channel_init(SIM_channel_t *channel, uint32_t oid, uint32_t cid, uint32_t wid)
{
	channel->oid = oid;
	channel->cid = cid;
	channel->wid = wid;
}

bool SIM_channel_get_has_packet(SIM_channel_t *channel)
{
	assert(channel);
	return channel->has_packet;
}


bool SIM_channel_get_output(SIM_channel_t *channel)
{
	assert(channel);
	return channel->output;
}

bool SIM_channel_set_packet(SIM_channel_t *channel, SIM_packet_t packet)
{
	assert(channel);

	if(SIM_channel_get_has_packet(channel))
	{
		return false;
	}
	channel->packet = packet;
	channel->has_packet = true;
	return true;
}


bool SIM_channel_get_packet(SIM_channel_t *channel, SIM_packet_t *packet)
{
	assert(channel);

	if(!SIM_channel_get_has_packet(channel))
	{
		return false;
	}
	*packet = channel->packet;
	channel->has_packet = false;
	return true;
}