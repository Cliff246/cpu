#ifndef __WS_SIM_MAILBOX_HEADER__
#define __WS_SIM_MAILBOX_HEADER__

#include "OBJ_bundle.h"
#include "SIM_commons.h"
#include "SIM_packet.h"
#include "SIM_mail.h"

typedef struct WS_SIM_mailbox
{
	SIM_channel_global_t channel;


	bool has_mailout;
	bool done_input;

	SIM_mail_t mailin;
	SIM_mail_t mailout;
}SIM_mailbox_t;

typedef struct WS_SIM_graph SIM_graph_t;


bool SIM_mailbox_recieving(SIM_graph_t *graph, SIM_mailbox_t *mailbox);
bool SIM_mailbox_sending(SIM_graph_t *graph, SIM_mailbox_t *mailbox);

bool SIM_mailbox_import_channel(SIM_graph_t *graph, SIM_mailbox_t *mailbox);
bool SIM_mailbox_export_channel(SIM_graph_t *graph, SIM_mailbox_t *mailbox);


SIM_packet_t SIM_mailbox_dispatch_out(SIM_mailbox_t *mailbox);
void SIM_mailbox_schedule(SIM_mail_t *mail, SIM_graph_t *graph);

#endif