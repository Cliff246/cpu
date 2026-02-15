#include "OBJ_message.h"
#include "RUN_channel.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


void RUN_channel_init(RUN_chnl_t *channel, uint32_t wcid, uint32_t cid, uint32_t wid)
{
	//channel->wcid = wcid;
	//channel->cid = cid;
	//channel->wid = wid;
}

bool RUN_channel_get_has_packet(RUN_chnl_t *channel)
{
	assert(channel);
	return NULL;

}


bool RUN_channel_get_output(RUN_chnl_t *channel)
{
	assert(channel);
	return NULL;
}



void RUN_channel_print(RUN_chnl_t *channel)
{
}