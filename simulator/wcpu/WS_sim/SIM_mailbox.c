#include "SIM_channel.h"
#include "SIM_commons.h"
#include "SIM_graph.h"
#include "SIM_mail.h"
#include "SIM_mailbox.h"
#include "SIM_packet.h"
#include "SIM_wire.h"
#include <assert.h>


bool SIM_mailbox_recieving(SIM_graph_t *graph, SIM_mailbox_t *mailbox)
{
	assert(graph);
	assert(mailbox);
	SIM_channel_global_t local_channel_global = mailbox->channel;

	SIM_channel_t *channel = SIM_graph_get_channel(graph, local_channel_global);

	SIM_wire_global_t wire_global = channel->wid;

	SIM_wire_t *wire = SIM_graph_get_wire(graph, wire_global);


	SIM_wire_channel_t wire_channel =  SIM_wire_bus_get_output(wire);
	SIM_channel_global_t dest_channel_global =  SIM_wire_channel_convert(wire, wire_channel);

	SIM_channel_t *dest_channel = SIM_graph_get_channel(graph, dest_channel_global);

	if(dest_channel == channel)
		return true;
	return false;

}


bool SIM_mailbox_sending(SIM_graph_t *graph, SIM_mailbox_t *mailbox)
{
assert(graph);
	assert(mailbox);
	SIM_channel_global_t local_channel_global = mailbox->channel;

	SIM_channel_t *channel = SIM_graph_get_channel(graph, local_channel_global);

	SIM_wire_global_t wire_global = channel->wid;

	SIM_wire_t *wire = SIM_graph_get_wire(graph, wire_global);


	SIM_wire_channel_t wire_channel =  SIM_wire_bus_get_input(wire);
	SIM_channel_global_t input_channel_global =  SIM_wire_channel_convert(wire, wire_channel);

	SIM_channel_t *input_channel = SIM_graph_get_channel(graph, input_channel_global);

	if(input_channel == channel)
		return true;
	return false;

}



bool SIM_mailbox_import_channel(SIM_graph_t *graph, SIM_mailbox_t *mailbox)
{
	SIM_channel_t *channel = SIM_graph_get_channel(graph, mailbox->channel);
	assert(channel);
	SIM_packet_t packet = {0};
	bool import = SIM_channel_get_packet(channel, &packet);
	if(!import)
		return false;



	//try to write a packet to the mailbox
	bool write_test = SIM_mail_write_packet(&mailbox->mailin, packet);
	if(write_test == true)
	{
		return true;
	}
	return false;
}

//export channel is open
bool SIM_mailbox_export_channel(SIM_graph_t *graph, SIM_mailbox_t *mailbox, OBJ_msg_t *msg)
{
	SIM_channel_t *channel = SIM_graph_get_channel(graph, mailbox->channel);
	assert(channel);



	bool read_test = SIM_mail_can_read_packet(&mailbox->mailout);

	if(read_test == false)
		assert(0);
	SIM_mail_put_packets(&mailbox->mailout, msg);
	//bool import = SIM_channel_set_packet(channel, packet);
	//if(!import)
	//	return false;
	assert(0 && "TODO");
	return false;
}

