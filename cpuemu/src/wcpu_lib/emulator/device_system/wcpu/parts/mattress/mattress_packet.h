#ifndef __MATRESS_PACKET_HEADER__
#define __MATRESS_PACKET_HEADER__

#include <stdbool.h>

#define MATTRESS_PACKET_SIZE 2

typedef struct mattress_interface_packet
{
	int mail_src;
	int mail_dest;
	int64_t content[MATTRESS_PACKET_SIZE];
	bool tracked;
	bool sent;

}mat_pkt_t;





#endif