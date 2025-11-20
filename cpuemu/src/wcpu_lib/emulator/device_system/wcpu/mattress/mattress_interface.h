#ifndef __MATTRESS_INTERFACE__
#define __MATTRESS_INTERFACE__


#include "mattress_packet.h"



typedef enum mattress_interface_state
{
	MATTRESS_INTERFACE_INVAL,
	MATTRESS_INTERFACE_WORKING,
	MATTRESS_INTERFACE_EMPTY,
	MATTRESS_INTERFACE_MOVING,
	MATTRESS_INTERFACE_ERROR

}mattress_interface_state_t;

#define MATTRESS_INTERFACE_PACKET_LIMIT 10


typedef struct mattress_port
{
	mattress_interface_state_t state;

	//error code
	int error;
	int mailbox_src;



	mat_pkt_t *buffer_in[MATTRESS_INTERFACE_PACKET_LIMIT];
	mat_pkt_t *buffer_out[MATTRESS_INTERFACE_PACKET_LIMIT];



}mat_port_t;

mat_port_t *create_matrix_port(int src);

bool has_port_changed(mat_port_t *port);

mat_pkt_t *read_port_packet(mat_port_t *port);
bool write_port_packet(mat_port_t *port, mat_pkt_t *pkt);


#endif