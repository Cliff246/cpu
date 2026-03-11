#ifndef __SCENE_TOPOLOGY_HEADER__
#define __SCENE_TOPOLOGY_HEADER__

#include <stdint.h>

#include "SCENE_link.h"

typedef struct SCENE_topology
{
	uint64_t links_count;
	SCENE_link_t *links;
}SCENE_topology_t;

#endif