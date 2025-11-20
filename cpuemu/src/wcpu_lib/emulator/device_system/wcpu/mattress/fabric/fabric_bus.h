#ifndef __FABRIC_BUS_HEADER__
#define __FABRIC_BUS_HEADER__


#include "mattress_interface.h"
#include "mattress_packet.h"

typedef struct fabric_bus
{
	mat_pkt_t pkt_out;
	mat_pkt_t pkt_in;

	int last_cycle_in;
	int last_cycle_out;



	int has_input;
	int has_output;

}fabric_bus_t;







#endif