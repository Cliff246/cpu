#include "OBJ_message.h"
#include "SIM_channel.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


void SIM_channel_init(SIM_channel_t *channel, uint32_t wcid, uint32_t cid, uint32_t wid)
{
	//channel->wcid = wcid;
	//channel->cid = cid;
	//channel->wid = wid;
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



void SIM_channel_print(SIM_channel_t *channel)
{
	printf("id:%ld has:%d wire:%p device:%p\n", channel->id, channel->has_packet, channel->wire, channel->device);
}