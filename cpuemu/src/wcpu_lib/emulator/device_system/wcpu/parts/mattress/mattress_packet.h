#ifndef __WCPU_PART_MATRESS_PACKET_HEADER__
#define __WCPU_PART_MATRESS_PACKET_HEADER__

#include <stdbool.h>
#include <stdint.h>

#define MATTRESS_PACKET_SIZE 2

typedef struct mattress_interface_packet
{

	int src, dst;
	int time;
	int64_t content[MATTRESS_PACKET_SIZE];
	bool tracked;
	bool sent;
	bool active;
}mattress_packet_t;





#endif