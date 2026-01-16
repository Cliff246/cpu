#ifndef __WS_SIM_WIRESLOT_HEADER__
#define __WS_SIM_WIRESLOT_HEADER__

#include <stdint.h>
#include <stdbool.h>

#include "OBJ_message.h"
typedef struct WS_SIM_wireslot
{
	//packet index
	OBJ_msg_t msg;
	bool used;

}SIM_wireslot_t;



#endif
