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
	SIM_mailbox_t mailboxs[OBJ_MAX_CHANNELS];
	SIM_routemap_t routemap;
}SIM_port_t;


typedef struct WS_SIM_graph SIM_graph_t;

//void SIM_port_update_channels(SIM_graph_t *graph,

#endif