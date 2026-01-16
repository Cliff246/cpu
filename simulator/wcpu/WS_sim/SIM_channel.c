#include "OBJ_message.h"
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
