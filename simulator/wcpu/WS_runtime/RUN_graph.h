#ifndef __RUN_GRAPH_HEADER__
#define __RUN_GRAPH_HEADER__

#include "RUN_addrtbl.h"
#include "RUN_channel.h"
#include "RUN_object.h"
#include "RUN_pools.h"
#include "RUN_idmap.h"
#include "SIM_stage.h"
#include <stdint.h>

typedef struct RUN_graph
{

	const SIM_stage_t *stage;
	RUN_idmap_t idmap;
	RUN_addrtbl_t addrtbl;
	RUN_pool_t pool;


}RUN_graph_t;
//alloc stage
bool RUN_alloc_graph(RUN_graph_t *graph);


bool RUN_alloc_graph_idmap(RUN_graph_t *graph);
bool RUN_alloc_graph_pool(RUN_graph_t *graph);
bool RUN_alloc_graph_addrtbl(RUN_graph_t *graph);

//build stage

bool RUN_build_graph_idmap(RUN_graph_t *graph);
bool RUN_build_graph_pool(RUN_graph_t *graph);
bool RUN_build_graph_addrtbl(RUN_graph_t *graph);


bool RUN_build_graph(RUN_graph_t *graph);


#endif