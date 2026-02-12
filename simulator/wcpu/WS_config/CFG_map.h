#ifndef __WS_CFG_MAP_HEADER__
#define __WS_CFG_MAP_HEADER__

#include "SIM_channel.h"
#include "CFG_link.h"
#include "SIM_commons.h"
#include <stdint.h>

typedef struct CFG_context CFG_context_t;



typedef struct CFG_mapelm
{
	CFG_node_t *device;
	bool has_address;
	uint64_t address;
	uint64_t length;
	bool pretag;
	SIM_dtag_t tag;

}CFG_mapelm_t;


void CFG_print_mapelm(CFG_mapelm_t *elm);
CFG_node_id_t CFG_init_mapelm(CFG_mapelm_t *elm, CFG_node_t *node);


typedef struct CFG_mapkey
{
	CFG_mapelm_t *elm;
	uint64_t base;
	uint64_t size;
}CFG_mapkey_t;

bool CFG_init_mapkey(CFG_mapkey_t *key, CFG_mapelm_t *elm);
static inline bool CFG_overlap_mapkey(CFG_mapkey_t *key1, CFG_mapkey_t *key2);
static void CFG_swap_mapkey_list(CFG_mapkey_t *keys, uint64_t i, uint64_t j);
static uint64_t CFG_partition_mapkey_list(CFG_mapkey_t *list, uint64_t lo, uint64_t hi);
static void CFG_qsort_mapkey_list(CFG_mapkey_t *list, uint64_t lo, uint64_t hi);
static CFG_mapkey_t *CFG_bsearch_mapkey_list(CFG_mapkey_t *list, uint64_t size, uint64_t i);
static bool CFG_check_no_collision_mapkey_list(CFG_mapkey_t *list, uint64_t size);

typedef struct CFG_map
{
	uint64_t count;
	CFG_mapelm_t *table;

	CFG_mapelm_t **tagmap;


	uint64_t addrmap_count;
	CFG_mapkey_t *addrmap;

}CFG_map_t;


CFG_map_t *CFG_init_map(CFG_context_t *ctx);
static void CFG_init_tags_map(CFG_map_t *table, CFG_context_t *ctx);
static void CFG_init_tagmap_map(CFG_map_t  *table, CFG_context_t *ctx);
static void CFG_init_addrmap_map(CFG_map_t  *table, CFG_context_t *ctx);
static bool CFG_init_addrmap_align_map(CFG_map_t  *table, CFG_context_t *ctx);

bool CFG_add_from_key_map(CFG_map_t *table, CFG_mapelm_t *elm, CFG_node_id_t tag);
CFG_mapelm_t *CFG_get_from_key_map(CFG_map_t *table, CFG_node_id_t tag);
CFG_mapelm_t *CFG_get_from_addr_map(CFG_map_t *table, uint64_t addr);
CFG_mapelm_t *CFG_get_from_index_map(CFG_map_t *table, uint64_t i);
void CFG_free_map(CFG_map_t *table);

void CFG_print_table_map(CFG_map_t *table);
void CFG_print_tagmap_map(CFG_map_t *table);
void CFG_print_addrmap_map(CFG_map_t *table);
#endif