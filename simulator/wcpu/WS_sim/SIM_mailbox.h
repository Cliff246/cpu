#ifndef __WS_SIM_MAILBOX_HEADER__
#define __WS_SIM_MAILBOX_HEADER__

#include "OBJ_bundle.h"
#include "SIM_commons.h"
#include "SIM_packet.h"
#include "SIM_mail.h"

typedef struct WS_SIM_mailbox
{
	SIM_channel_global_t channel;

	SIM_mail_t mail;
}SIM_mailbox_t;

#endif