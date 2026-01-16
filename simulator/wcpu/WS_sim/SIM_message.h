#ifndef __WS_SIM_MESSAGE_HEADER__
#define __WS_SIM_MESSAGE_HEADER__

#include "OBJ_bundle.h"
#include "OBJ_message.h"

typedef struct WS_SIM_message
{
	OBJ_msg_t msg[OBJ_BUNDLE_SIZE];
}SIM_message_t;

#endif