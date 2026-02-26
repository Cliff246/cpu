#ifndef __SCENE_ANCHOR_HEADER__
#define __SCENE_ANCHOR_HEADER__



#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>



typedef struct SCENE_wire SCENE_wire_t;
typedef struct SCENE_device SCENE_device_t;

//link object between devices and wires
typedef struct SCENE_anchor
{
	uint64_t count;
	SCENE_wire_t *wire;
	SCENE_device_t *devices[];

}SCENE_anchor_t;

//allocate a single sized anchor
SCENE_anchor_t *SCENE_alloc_anchor(uint64_t count);


bool SCENE_init_anchor(SCENE_anchor_t *anchor);

//dangerous and can leave dangly boys
void SCENE_free_anchor(SCENE_anchor_t *anchor);

#endif
