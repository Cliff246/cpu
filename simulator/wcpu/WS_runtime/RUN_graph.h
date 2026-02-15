#ifndef __RUN_GRAPH_HEADER__
#define __RUN_GRAPH_HEADER__

#include "RUN_channel.h"
#include "RUN_object.h"
#include "RUN_pools.h"
#include "RUN_idmap.h"
#include "SIM_stage.h"
#include <stdint.h>

typedef struct RUN_graph
{

	SIM_stage_t *stage;
	RUN_idmap_t idmap;

	RUN_pool_t pool;


}RUN_graph_t;

void RUN_generate_graph_idmap(RUN_graph_t *graph, SIM_stage_t *stage);
void RUN_generate_graph_pool(RUN_graph_t *graph, SIM_stage_t *stage);

RUN_graph_t *RUN_alloc_graph(SIM_stage_t *stage);
bool RUN_build_graph(RUN_graph_t *graph);


#endif