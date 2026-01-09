#include "SIM_channel.h"
#include <stdint.h>

SIM_channel_t SIM_channel_init(uint32_t entry_id, uint32_t port_id)
{
	SIM_channel_t channel =
	{
		.entry_id = entry_id,
		.port_id = port_id
	};

	return channel;

}