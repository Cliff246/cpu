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
	//OBJ_bundle_t bout;

	uint32_t mailboxes_size;

	SIM_mailbox_t *mailboxes;
	SIM_routemap_t routemap;
}SIM_port_t;

SIM_port_t *SIM_port_init();
/*
typedef struct WS_SIM_graph SIM_graph_t;
SIM_mailbox_t *SIM_port_get_mailbox(SIM_port_t *port, SIM_channel_local_t local);



//produces a bundles from the read channels
bool SIM_port_produce_bundle(SIM_graph_t *graph, SIM_port_t *port, OBJ_bundle_t *bundle);
bool SIM_port_recieve_bundle(SIM_graph_t *graph, SIM_port_t *port, OBJ_bundle_t *bundle);



*/



#endif