#ifndef __SCENE_TOPOLOGY_HEADER__
#define __SCENE_TOPOLOGY_HEADER__

#include <stdint.h>

#include "SCENE_link.h"
#include "SIM_commons.h"
#include "hashmap.h"

typedef struct SCENE_context SCENE_context_t;
typedef struct SCENE_port SCENE_port_t;




typedef struct SCENE_topology
{
	//context links array
	uint64_t links_count;
	uint64_t links_alloc;
	SCENE_link_t **links_list;

	//context ports array
	uint64_t ports_alloc;
	uint64_t ports_count;
	SCENE_port_t **ports_list;


	bool links_finished;
	bool links_symbolized;
	p_hashtable_t links_table_positive;
}SCENE_topology_t;

SCENE_topology_t *SCENE_init_topology(void);

uint64_t SCENE_append_link_topology(SCENE_topology_t *topology, SCENE_link_t *link);
uint64_t SCENE_append_port_topology(SCENE_topology_t *topology, SCENE_port_t *port);

void SCENE_print_topology(SCENE_topology_t *topology);
void SCENE_finalize_links_topology(SCENE_topology_t *topology);

void SCENE_symbolize_topology(SCENE_topology_t *topology, SCENE_context_t *context);

#endif