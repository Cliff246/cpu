#include "SIM_port.h"
#include "OBJ_bundle.h"
#include "SIM_graph.h"
#include "SIM_mailbox.h"
#include <assert.h>


SIM_mailbox_t *SIM_port_get_mailbox(SIM_port_t *port, SIM_channel_local_t local)
{
	assert(port);
	assert(local >= 0 && local < port->mailboxes_size);
	SIM_mailbox_t *mailbox = &port->mailboxes[local];
	return mailbox;
}

//make a bundle
bool SIM_port_produce_bundle(SIM_graph_t *graph, SIM_port_t *port, OBJ_bundle_t *bundle)
{

	assert(graph && port && bundle);
	for(int i = 0; i < port->mailboxes_size; ++i)
	{
		SIM_mailbox_t *mailbox = SIM_port_get_mailbox(port, i);
		assert(mailbox);

		if(!SIM_mailbox_recieving(graph, mailbox))
			continue;
		SIM_channel_t *channel = SIM_graph_get_channel(graph, mailbox->channel);
		
	}
}


//collect from the port and make it a huge bundles
bool SIM_port_recieve_bundle(SIM_graph_t *graph, SIM_port_t *port, OBJ_bundle_t *bundle)
{
	assert(graph && port && bundle);
	for(int i = 0; i < port->mailboxes_size; ++i)
	{
		SIM_mailbox_t *mailbox = SIM_port_get_mailbox(port, i);
		assert(mailbox);

	}
}
