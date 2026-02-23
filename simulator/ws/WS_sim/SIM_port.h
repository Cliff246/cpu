#ifndef __SIM_PORT_HEADER__
#define __SIM_PORT_HEADER__

#include "SIM_anchor.h"
#include <stdint.h>

//resource goes to port, port ->

typedef struct SIM_port
{
	uint64_t count;
	SIM_anchor_t **anchors;
}SIM_port_t;
#endif