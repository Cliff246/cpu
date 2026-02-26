#ifndef __RUN_ADDRESSTABLE_HEADER__
#define __RUN_ADDRESSTABLE_HEADER__

#include "RUN_commons.h"
#include "SCENE_scene.h"

#include <stdint.h>

typedef struct RUN_address_elment
{
	RUN_objid_t id;
	uint64_t base;
	uint64_t size;
}RUN_addrelm_t;

typedef struct RUN_address_table
{
	//empty
}RUN_addrtbl_t;


//allocates size many addresses to be able to search
void RUN_alloc_addrtbl(RUN_addrtbl_t *tbl, uint64_t size);

//builds address table
void RUN_build_addrtbl(RUN_addrtbl_t *tbl, const SCENE_scene_t *scene);


#endif
