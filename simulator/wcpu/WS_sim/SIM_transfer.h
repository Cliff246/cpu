#ifndef __WS_SIM_WIRESLOT_HEADER__
#define __WS_SIM_WIRESLOT_HEADER__

#include <stdint.h>
#include <stdbool.h>

#include "OBJ_message.h"
#include "SIM_packet.h"


typedef struct WS_SIM_transfer
{
	//packet index
	SIM_packet_t packet;
	bool used;

}SIM_transfer_t;



#endif
