#ifndef __SIM_ANCHOR_HEADER__
#define __SIM_ANCHOR_HEADER__



#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>



typedef struct SIM_wire SIM_wire_t;
typedef struct SIM_device SIM_device_t;

//link object between devices and wires
typedef struct SIM_anchor
{
	uint64_t count;
	SIM_wire_t *wire;
	SIM_device_t *devices[];

}SIM_anchor_t;

//allocate a single sized anchor
SIM_anchor_t *SIM_alloc_anchor(uint64_t count);


bool SIM_init_anchor(SIM_anchor_t *anchor);

//dangerous and can leave dangly boys
void SIM_free_anchor(SIM_anchor_t *anchor);

#endif