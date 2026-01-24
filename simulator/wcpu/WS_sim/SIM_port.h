#ifndef __WS_SIM_PORT_HEADER__
#define __WS_SIM_PORT_HEADER__

#include "OBJ_bundle.h"
#include "OBJ_constants.h"
#include "SIM_mailbox.h"
#include "SIM_routemap.h"
#include "SIM_commons.h"


#include <stdint.h>
#include <stdbool.h>


typedef struct WS_SIM_port
{
	uint16_t object_id;
	uint8_t mailboxes_size;
	int8_t mailboxes_read_index, mailboxes_write_index;


	

	SIM_mailbox_t mailboxes[OBJ_MAX_CHANNELS];
	SIM_routemap_t routemap;
}SIM_port_t;


typedef struct WS_SIM_graph SIM_graph_t;
SIM_mailbox_t *SIM_port_get_mailbox(SIM_port_t *port, SIM_channel_local_t local);

//read the channels
void SIM_port_read_channels(SIM_graph_t *graph, SIM_port_t *port);

//produces a bundles from the read channels
void SIM_port_produce_bundle(SIM_graph_t *graph, SIM_port_t *port);




//collects the bundles given and outputs it onto the proper channels
void SIM_port_collect_bundle(SIM_graph_t *graph, SIM_port_t *port);

//write channels
void SIM_port_write_channels(SIM_graph_t *graph, SIM_port_t *port);




#endif