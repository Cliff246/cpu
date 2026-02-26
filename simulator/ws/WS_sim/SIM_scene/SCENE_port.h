#ifndef __SCENE_PORT_HEADER__
#define __SCENE_PORT_HEADER__

#include "SCENE_anchor.h"
#include <stdint.h>

//resource goes to port, port ->

typedef struct SCENE_port
{
	uint64_t count;
	SCENE_anchor_t **anchors;
}SCENE_port_t;
#endif
