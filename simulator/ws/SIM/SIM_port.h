#ifndef __WS_SIMULATOR_PORT__
#define __WS_SIMULATOR_PORT__

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
//#include "device.h"
//#include "device_message.h"

typedef struct WS_simulator_packet
{
	WS_dev_msg_t *msg;
	int latency;
}WS_sim_pkt_t;




#endif