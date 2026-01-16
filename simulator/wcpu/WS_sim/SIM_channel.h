#ifndef __WS_SIM_CHANNEL_HEADER__
#define __WS_SIM_CHANNEL_HEADER__


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
	//channel id
	uint8_t cid;
	OBJ_msg_t msg;
	bool has;
}SIM_channel_t;

void SIM_channel_init(SIM_channel_t *channel, uint32_t oid, uint32_t cid);


#endif