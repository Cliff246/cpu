#ifndef __WS_SIM_MESSAGE_HEADER__
#define __WS_SIM_MESSAGE_HEADER__

#include "OBJ_bundle.h"
#include "OBJ_message.h"
#include "SIM_commons.h"

#include <stdint.h>
#include <stdalign.h>
#include <stdbool.h>

typedef struct WS_SIM_message
{
	SIM_tag_t tag;
}SIM_message_t;


#endif