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
	SIM_mailbox_t mailboxes[OBJ_MAX_CHANNELS];
	SIM_routemap_t routemap;
}SIM_port_t;


typedef struct WS_SIM_graph SIM_graph_t;

void SIM_port_update_channels(SIM_graph_t *graph, SIM_port_t *port);

#endif