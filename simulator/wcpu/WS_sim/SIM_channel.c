#include "OBJ_message.h"
#include "SIM_channel.h"
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>


void SIM_channel_init(SIM_channel_t *channel, uint32_t oid, uint32_t cid)
{
	channel->oid = oid;
	channel->cid = cid;

}

