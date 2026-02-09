#include "SIM_port.h"
#include "OBJ_bundle.h"
#include "OBJ_message.h"
#include "SIM_channel.h"
#include "SIM_commons.h"
#include "SIM_graph.h"
#include "SIM_mail.h"
#include "SIM_mailbox.h"
#include "SIM_routemap.h"
#include "SIM_wire.h"
#include <assert.h>



SIM_port_t *SIM_port_init()
{
	SIM_port_t *port = calloc(1, sizeof(SIM_port_t));

	assert(port);

	return port;
}
/*

SIM_mailbox_t *SIM_port_get_mailbox(SIM_port_t *port, SIM_channel_local_t local)
{
	assert(port);
	assert(local >= 0 && local < port->mailboxes_size);
	//SIM_mailbox_t *mailbox = &port->mailboxes[local];
	//return mailbox;
}

//make a bundle
bool SIM_port_produce_bundle(SIM_graph_t *graph, SIM_port_t *port, OBJ_bundle_t *bundle)
{
	int current_bundle_pos = 0;
	assert(graph && port && bundle);
	for(int i = 0; i < port->mailboxes_size; ++i)
	{
		SIM_mailbox_t *mailbox = SIM_port_get_mailbox(port, i);
		assert(mailbox);

		if(!SIM_mailbox_recieving(graph, mailbox))
			continue;
		SIM_channel_t *channel = SIM_graph_get_channel(graph, mailbox->channel);


		bool can_import = SIM_mailbox_import_channel(graph, mailbox);
		if(can_import == false)
		{
			OBJ_msg_t msg = SIM_mail_pull_packets(&mailbox->mailin);

			bundle->msg[current_bundle_pos++] = msg;
			if(current_bundle_pos == 8)
			{
				assert(0);
			}
		}
	}
	return true;
}


//collect from the port and make it a huge bundles
bool SIM_port_recieve_bundle(SIM_graph_t *graph, SIM_port_t *port, OBJ_bundle_t *bundle)
{
	assert(graph && port && bundle);
	assert("todo");
	for(int k = 0; k < 8; ++k)
	{
		OBJ_msg_t *msg = &bundle->msg[k];

		if(msg->type != OBJ_MSG_INVAL)
		{


		}

		if(msg->type == OBJ_MSG_LOAD)
		{
		 	int32_t valid = SIM_routemap_search(&port->routemap, msg->value.address);
			if(valid == -1)
				assert(0);
			SIM_route_t *route = &port->routemap.map[valid];

			for(int i = 0; i < route->length; ++i)
			{
				assert(i < port->mailboxes_size);
				SIM_mailbox_t *mailbox = SIM_port_get_mailbox(port, i);
				assert(mailbox);

				SIM_channel_t *channel = SIM_graph_get_channel(graph, mailbox->channel);
				SIM_wire_channel_t wire_channel = channel->wcid;
				SIM_wire_t *wire = SIM_graph_get_wire(graph, channel->wid);

				if(!SIM_mailbox_sending(graph, mailbox))
				{
					SIM_wire_bus_add_channel_roundrobin(wire, wire_channel);
				}

				bool can_export = SIM_mailbox_export_channel(graph, mailbox, &bundle->msg[k]);
				if(can_export == false)
				{

				}
			}
		}

	}
	return true;
}

*/