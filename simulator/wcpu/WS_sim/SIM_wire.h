#ifndef __SIM_WIRE_HEADER__
#define __SIM_WIRE_HEADER__

#include "CFG_link.h"
#include "SIM_commons.h"
#include "SIM_device.h"
#include "SIM_anchor.h"
#include <stdint.h>

typedef struct SIM_wire
{

	SIM_wid_t wid;
	SIM_wkey_t wkey;
	uint64_t latency;
	uint64_t throughput;

	SIM_anchor_t *anchor;

}SIM_wire_t;

SIM_wire_t *SIM_alloc_wire();
void SIM_init_wire(SIM_wire_t *wire, CFG_edge_t *edge);
void SIM_resolve_wire(SIM_wire_t *wire);
void SIM_build_wire(SIM_wire_t *wire);

#endif