#ifndef __WS_SIM_CHANNEL_HEADER__
#define __WS_SIM_CHANNEL_HEADER__

#include <stdint.h>
#include <stdbool.h>



//control unit for each end port
//designed to hold and manage merging and more
typedef struct WS_SIM_channel
{
	//entry id
	uint32_t entry_id;
	//port id
	uint32_t port_id;


}SIM_channel_t;


SIM_channel_t SIM_channel_init(uint32_t entry_id, uint32_t port_id);

#endif