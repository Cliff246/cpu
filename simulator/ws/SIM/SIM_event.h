#ifndef __WS_SIMULATOR_EVENT_HEADER__
#define __WS_SIMULATOR_EVENT_HEADER__

#include <stdint.h>
#include <stdbool.h>

typedef struct WS_simulator_event_vtable
{

}WS_sim_evnt_vt_t;

typedef struct WS_simulator_event
{
	int timing;

	WS_sim_evnt_vt_t *vt;

}WS_sim_evnt_t;


#endif