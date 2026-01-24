#include "SIM_port.h"
#include "SIM_mailbox.h"
#include <assert.h>


SIM_mailbox_t *SIM_port_get_mailbox(SIM_port_t *port, SIM_channel_local_t local)
{
	assert(port);
	assert(local >= 0 && local < port->mailboxes_size);
	SIM_mailbox_t *mailbox = &port->mailboxes[local];
	return mailbox;
}

void SIM_port_read_channels(SIM_graph_t *graph, SIM_port_t *port)
{
	for(int i = 0; i < port->mailboxes_size; ++i)
	{
		SIM_mailbox_t *mailbox = SIM_port_get_mailbox(port, i);
	}
}

void SIM_port_collect_bundles(SIM_graph_t *graph, SIM_port_t *port)
{

}

void SIM_port_write_channels(SIM_graph_t *graph, SIM_port_t *port)
{
	for(int i = 0; i < port->mailboxes_size; ++i)
	{
		SIM_mailbox_t *mailbox = SIM_port_get_mailbox(port, i);

	}
}