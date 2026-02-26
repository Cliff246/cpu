#ifndef __SCENE_WIRE_HEADER__
#define __SCENE_WIRE_HEADER__

#include "CFG_link.h"
#include "SIM_commons.h"
#include "SCENE_device.h"
#include "SCENE_anchor.h"
#include <stdint.h>

typedef struct SCENE_wire
{

	SIM_wid_t wid;
	SIM_wkey_t wkey;
	uint64_t latency;
	uint64_t throughput;

	SCENE_anchor_t *anchor;

}SCENE_wire_t;

SCENE_wire_t *SCENE_alloc_wire();
void SCENE_init_wire(SCENE_wire_t *wire, CFG_edge_t *edge);
void SCENE_resolve_wire(SCENE_wire_t *wire);
void SCENE_build_wire(SCENE_wire_t *wire);

#endif
