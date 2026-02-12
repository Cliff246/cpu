#ifndef __CFG_EDGE_HEADER__
#define __CFG_EDGE_HEADER__

#include "CFG_commons.h"
#include "CFG_link.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct CFG_edge
{
	CFG_edge_id_t id;
	uint64_t latency;
	uint64_t throughput;
	uint64_t channels_alloc;
	uint64_t channels_count;
	CFG_link_id_t *channels;
	struct
	{
		bool added_channels;
		bool finished;
	}done;
}CFG_edge_t;


void CFG_init_edge(CFG_edge_t *edge, CFG_edge_id_t id, uint64_t latency, uint64_t throughput);

void CFG_print_edge(CFG_edge_t *wireconfig);
void CFG_append_edge(CFG_edge_t  *wireconfig, CFG_node_id_t cid);


void CFG_free_edge(CFG_edge_t *ptr);


#endif