#include "SIM_wire.h"
#include "CFG_edge.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>


SIM_wire_t *SIM_alloc_wire()
{
	SIM_wire_t *wire = calloc(1, sizeof(SIM_wire_t));
	assert(wire);
	return wire;
}

void SIM_init_wire(SIM_wire_t *wire, CFG_edge_t *edge)
{
	wire->latency = edge->latency;
	wire->throughput = edge->throughput;
	wire->wkey = edge->id;


}

void SIM_resolve_wire(SIM_wire_t *wire)
{

}

void SIM_build_wire(SIM_wire_t *wire)
{

}