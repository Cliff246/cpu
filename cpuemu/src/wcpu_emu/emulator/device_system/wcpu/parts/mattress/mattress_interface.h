#ifndef __WCPU_PART_MATTRESS_INTERFACE_HEADER__
#define __WCPU_PART_MATTRESS_INTERFACE_HEADER__


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


typedef struct wcpu_mattress_interface
{
	mattress_interface_state_t state;
	//error code
	int error;
	//source
	int mailbox_src;
	mattress_packet_t *buffer_in[MATTRESS_INTERFACE_PACKET_LIMIT];
	mattress_packet_t *buffer_out[MATTRESS_INTERFACE_PACKET_LIMIT];
}mattress_interface_t;

mattress_interface_t *create_matrix_interface(int src);

bool has_interface_changed(mattress_interface_t *interface);

mattress_packet_t *read_interface_packet(mattress_interface_t *interface);
bool write_interface_packet(mattress_interface_t *interface, mattress_packet_t *pkt);


#endif