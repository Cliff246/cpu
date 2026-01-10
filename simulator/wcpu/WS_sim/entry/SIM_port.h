#ifndef __WS_SIMULATOR_PORT__
#define __WS_SIMULATOR_PORT__



#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef enum WS_SIM_port_state
{
	SIM_PORT_STATE_SENDING,
	SIM_PORT_STATE_WAITING,
	SIM_PORT_STATE_RECIEVING,
	SIM_PORT_STATE_READY,
	SIM_PORT_STATE_EMPTY,
}SIM_port_state_t;


typedef struct WS_SIM_port
{
	SIM_port_state_t state;
	int wire_id;



}SIM_port_t;





#endif
