#ifndef __WS_SIM_PORT_HEADER__
#define __WS_SIM_PORT_HEADER__

#include "OBJ_bundle.h"
#include "OBJ_constants.h"
#include "SIM_message.h"
#include "SIM_routetable.h"

#include <stdint.h>
#include <stdbool.h>

#define SIM_PORT_SIZE 64

typedef struct WS_SIM_msgkey
{
	uint64_t group;
	int16_t index;
}SIM_msgkey_t;

typedef struct WS_SIM_port
{
	uint16_t object_id;

	uint32_t channels[OBJ_MAX_CHANNELS];
	OBJ_bundle_t read;
	OBJ_bundle_t write;
	SIM_routetable_t routetable;
	SIM_msgkey_t keytable[SIM_PORT_SIZE];
	SIM_message_t msg[SIM_PORT_SIZE];
	SIM_message_t chnl_buffer[OBJ_MAX_CHANNELS];
}SIM_port_t;


typedef struct WS_SIM_graph SIM_graph_t;

//can read the msg buffer is not empty
bool SIM_port_can_read(SIM_graph_t *graph, SIM_port_t *port);
//can write, the msg buffer is not full and the
bool SIM_port_can_write(SIM_graph_t *graph, SIM_port_t *port);

//recieve and update any import chnl_buffer with new content
void SIM_port_channel_active_recieve(SIM_graph_t *graph, SIM_port_t *port);

//recieve any chnl_buffer information into msg
bool SIM_port_channel_recieve(SIM_graph_t *graph, SIM_port_t *port);
//release any avilable msg into channel buffer that can be updated
bool SIM_port_channel_release(SIM_graph_t *graph, SIM_port_t *port);

//release into any export channel the desired object
void SIM_port_channel_active_release(SIM_graph_t *graph, SIM_port_t *port);

//write any possible readable msg's into the bundle
int64_t SIM_port_bundle_write(SIM_port_t *port, OBJ_bundle_t *bundle);
//read from a bundle and put into msg's
void SIM_port_bundle_read(SIM_port_t *port, OBJ_bundle_t *bundle);

#endif