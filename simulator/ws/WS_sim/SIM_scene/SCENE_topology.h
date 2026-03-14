#ifndef __SCENE_TOPOLOGY_HEADER__
#define __SCENE_TOPOLOGY_HEADER__

#include <stdint.h>

#include "SCENE_link.h"
#include "hashmap.h"



typedef struct SCENE_topology
{
	uint64_t links_count;
	uint64_t links_alloc;
	SCENE_link_t **links_list;
	bool links_finished;
	bool links_symbolized;
	p_hashtable_t links_table_positive;
	p_hashtable_t links_table_negative;
}SCENE_topology_t;

SCENE_topology_t *SCENE_init_topology(void);

uint64_t SCENE_append_topology(SCENE_topology_t *topology, SCENE_link_t *link);
void SCENE_print_topology(SCENE_topology_t *topology);
void SCENE_finalize_links_topology(SCENE_topology_t *topology);

void SCENE_symbolize_topology(SCENE_topology_t *topology);

#endif