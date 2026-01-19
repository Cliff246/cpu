#ifndef __WS_SIM_MAILBOX_HEADER__
#define __WS_SIM_MAILBOX_HEADER__

#include "OBJ_bundle.h"
#include "SIM_commons.h"
#include "SIM_packet.h"
#include "SIM_mail.h"
#include "SIM_port.h"

typedef struct WS_SIM_mailbox
{
	SIM_channel_global_t channel;
	bool current_dir;
	bool ready;
	SIM_mail_t mailin;
	SIM_mail_t mailout;
}SIM_mailbox_t;

typedef struct WS_SIM_graph SIM_graph_t;

SIM_packet_t SIM_mailbox_dispatch_out(SIM_mailbox_t *mailbox);
void SIM_mailbox_schedule(SIM_mail_t *mail, SIM_graph_t *graph);

#endif