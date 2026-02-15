#ifndef __WS_CFG_NODE_HEADER__
#define __WS_CFG_NODE_HEADER__


#include "CFG_commons.h"
#include "CFG_entry.h"
#include "CFG_link.h"
#include "CFG_commons.h"
#include "hashmap.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdalign.h>
#include <sys/types.h>


typedef struct CFG_node_route
{
	uint64_t latency;
	uint64_t index;
	CFG_edge_id_t wire;
}CFG_node_route_t;

typedef struct CFG_node_route_row
{
	CFG_node_id_t tag; //target device identity
	uint64_t size; //size of the valid routes
	//per valid devconfig
	CFG_node_route_t *routes; //the routes
}CFG_node_route_row_t;


typedef struct CFG_node_route_table
{
	uint64_t tags_size; //the amount of devices
	//per device id
	CFG_node_route_row_t *tags; //the device route
}CFG_node_route_table_t;


typedef struct CFG_node
{
	//deep copied over
	_Alignas(128) p_hashtable_t initals;
	char *module;
	uint64_t chnlids_size;
	CFG_link_id_t *chnlids;
	bool has_address;
	uint64_t address;
	uint64_t length;
	bool has_pretag;
	CFG_node_id_t pretag;
	CFG_node_route_table_t routetable;
	struct
	{
		bool init_routetable;
		bool finished;
	}done;

}CFG_node_t;

bool CFG_add_prototag_node(CFG_node_t *node, CFG_prototag_t *tag);

bool CFG_prep_links_node(CFG_node_t *node,  CFG_link_buf_t *lbuf);

bool CFG_init_tags_node(CFG_node_t *node, CFG_entry_t *entry);

bool CFG_init_node(CFG_node_t *node, CFG_entry_t *entry, CFG_link_buf_t *lbuf);
void CFG_free_node(CFG_node_t *node);
uint64_t CFG_get_address_base_node(CFG_node_t *node);


bool CFG_has_address_range_node(CFG_node_t *node);

uint64_t CFG_get_address_size_node(CFG_node_t *node);
CFG_node_id_t CFG_has_pretag_node(CFG_node_t *node);
void CFG_init_routetable_node(CFG_node_t *node, uint64_t tags_size, CFG_node_id_t *ids, uint64_t edge_size);
void CFG_append_route_node(CFG_node_t *node, uint64_t index, CFG_node_route_t route);
void CFG_print_node(CFG_node_t *node);
CFG_node_id_t CFG_generate_tag(CFG_node_t *node);


#endif