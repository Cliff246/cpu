#ifndef __WS_SIM_MAIL_HEADER__
#define __WS_SIM_MAIL_HEADER__

#include "OBJ_message.h"
#include "SIM_packet.h"

typedef struct WS_SIM_mail
{
	uint64_t address;
	SIM_packet_t packets[];
}SIM_mail_t;

#endif
