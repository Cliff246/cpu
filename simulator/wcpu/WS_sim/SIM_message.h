#ifndef __WS_SIM_MESSAGE_HEADER__
#define __WS_SIM_MESSAGE_HEADER__

#include "SIM_messageblock.h"
#include "SIM_packet.h"
#include <stdint.h>


typedef struct WS_SIM_message
{
	SIM_messageblock_t *blocks;
}SIM_message_t;

#endif