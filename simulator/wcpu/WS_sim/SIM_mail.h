#ifndef __WS_SIM_MAIL_HEADER__
#define __WS_SIM_MAIL_HEADER__

#include "OBJ_message.h"
#include "SIM_packet.h"

typedef struct WS_SIM_mail
{
	int current;
	SIM_packet_t packet[8];
	OBJ_msg_t msg;
}SIM_mail_t;

#endif
