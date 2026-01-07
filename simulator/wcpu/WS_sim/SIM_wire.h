#ifndef __WS_SIM_WIRE_HEADER__
#define __WS_SIM_WIRE_HEADER__

#include <stdint.h>
#include <stdbool.h>

//sshould be fine
typedef struct WS_SIM_wirechannel
{
	//entry id
	uint32_t eid;
	//port id
	uint32_t pid;
}SIM_wirechannel_t;

typedef struct WS_SIM_wire
{
	//the index of the wire channel list
	uint32_t id;
	//amoount of time it takes for a packet to cross this
	uint32_t latency;
	uint64_t wirechannel_start;
	uint64_t wirechannel_size;

}SIM_wire_t;

#endif
