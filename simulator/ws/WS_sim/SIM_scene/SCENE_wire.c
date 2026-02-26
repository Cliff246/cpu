#include "SCENE_wire.h"
#include "CFG_edge.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>


SCENE_wire_t *SCENE_alloc_wire()
{
	SCENE_wire_t *wire = calloc(1, sizeof(SCENE_wire_t));
	assert(wire);
	return wire;
}

void SCENE_init_wire(SCENE_wire_t *wire, CFG_edge_t *edge)
{
	wire->latency = edge->latency;
	wire->throughput = edge->throughput;
	wire->wkey = edge->id;


}

void SCENE_resolve_wire(SCENE_wire_t *wire)
{

}

void SCENE_build_wire(SCENE_wire_t *wire)
{

}
