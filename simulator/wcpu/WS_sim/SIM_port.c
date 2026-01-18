#include "SIM_port.h"


//can read the msg buffer is not empty
bool SIM_port_can_read(SIM_graph_t *graph, SIM_port_t *port)
{

}
//can write, the msg buffer is not full and the
bool SIM_port_can_write(SIM_graph_t *graph, SIM_port_t *port)
{

}

//recieve and update any import chnl_buffer with new content
void SIM_port_channel_active_recieve(SIM_graph_t *graph, SIM_port_t *port)
{

}

//recieve any chnl_buffer information into msg
bool SIM_port_channel_recieve(SIM_graph_t *graph, SIM_port_t *port)
{

}
//release any avilable msg into channel buffer that can be updated
bool SIM_port_channel_release(SIM_graph_t *graph, SIM_port_t *port)
{

}

//release into any export channel the desired object
void SIM_port_channel_active_release(SIM_graph_t *graph, SIM_port_t *port)
{

}

//write any possible readable msg's into the bundle
int64_t SIM_port_bundle_write(SIM_port_t *port, OBJ_bundle_t *bundle)
{

}
//read from a bundle and put into msg's
void SIM_port_bundle_read(SIM_port_t *port, OBJ_bundle_t *bundle)
{

}