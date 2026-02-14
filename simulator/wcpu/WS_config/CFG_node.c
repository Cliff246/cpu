#include "CFG_node.h"
#include "CFG_link.h"
#include "CFG_commons.h"
#include "TAG_tag.h"
#include "TAG_int.h"
#include "TAG_list.h"
#include "TAG_string.h"
#include "TAG_bool.h"
#include "commons.h"
#include "hashmap.h"
#include <stdbool.h>
#include <stdalign.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


bool CFG_add_prototag_node(CFG_node_t *node, CFG_prototag_t *tag)
{
	//key should be coppied
	addto_hash_table(node->initals, tag->key, tag->tag);
	CFG_free_prototag(tag);
	return true;
}

bool CFG_init_tags_node(CFG_node_t *node, CFG_entry_t *entry)
{
	const uint64_t count = entry->size;

	const uint32_t standard = 100;
	if(count >= standard)
	{
		assert(0);
	}
	p_hashtable_t table = new_hash_table(standard, CFG_free_tag_hashtable);
	assert(table);


	node->initals = table;
	node->module = strdup(entry->module);
	assert(node->module);

	for(uint32_t i = 0; i < count; ++i)
	{
		CFG_prototag_t *proto = entry->prototags[i];
		assert(CFG_add_prototag_node(node, proto) == true);
	}
	CFG_free_entry(entry);
	return true;
}

bool CFG_prep_links_node(CFG_node_t *node,  CFG_link_buf_t *lbuf)
{
	TAG_tag_t *tag = getdata_from_hash_table(node->initals, "WIRE");
	assert(tag && "device must contain a wire tag");
	TAG_argptr_t get_size_arg =	TAG_get_fn(TAG_LIST, TAG_FN_LIST_GET_SIZE);
	TAG_argptr_t get_arg = TAG_get_fn(TAG_LIST, TAG_FN_LIST_GET);
	const uint64_t list_size = get_size_arg.LIST->get_size(tag);


	TAG_argptr_t get_int = TAG_get_fn(TAG_INT, TAG_FN_INT_GET);
	node->chnlids_size = list_size;

	CFG_edge_id_t wid[list_size];
	CFG_link_id_t *cid = calloc(list_size, sizeof(CFG_link_id_t));
	assert(cid);
	for(uint64_t i = 0; i < list_size; ++i)
	{
		TAG_tag_t *tmp =  get_arg.LIST->get(tag, i);
		assert(tmp->type == TAG_INT);
		CFG_link_id_t wire = get_int.INT->get(tmp);
		wid[i] = wire;
	}
	CFG_append_link_buf(lbuf, node, wid, list_size, cid);
	node->chnlids = cid;

	return true;
}

bool CFG_init_node(CFG_node_t *node, CFG_entry_t *entry, CFG_link_buf_t *lbuf)
{

	CFG_init_tags_node(node, entry);
	CFG_prep_links_node(node, lbuf);


	if(CFG_has_address_range_node(node))
	{
		node->has_address = true;
		node->address = CFG_get_address_base_node(node);
		node->length = CFG_get_address_size_node(node);
	}
	else
	{
		node->has_address = false;
	}
	CFG_node_id_t haspre = CFG_has_pretag_node(node);
	if(haspre >= 0)
	{
		node->has_pretag = true;
		node->pretag = haspre;
	}

	return true;
}
void CFG_free_node(CFG_node_t *node)
{
	free_hash_table(node->initals);
	free(node->module);
	free(node->chnlids);
	if(node->done.init_routetable)
	{

		for(uint64_t i = 0; i < node->routetable.tags_size; ++i)
		{
			free(node->routetable.tags[i].routes);
		}
		free(node->routetable.tags);
	}

}

bool CFG_has_address_range_node(CFG_node_t *node)
{

	TAG_tag_t *tag1 = getdata_from_hash_table(node->initals, "BASE");
	if(tag1 == NULL)
		return false;
	TAG_argptr_t get_base =	TAG_get_fn(TAG_INT, TAG_FN_INT_GET);

	TAG_tag_t *tag2 = getdata_from_hash_table(node->initals, "SIZE");
	if(tag2 == NULL)
		return false;
	TAG_argptr_t get_size =	TAG_get_fn(TAG_INT, TAG_FN_INT_GET);

	return true;
}
uint64_t CFG_get_address_base_node(CFG_node_t *node)
{

	TAG_tag_t *tag = getdata_from_hash_table(node->initals, "BASE");
	assert(tag != NULL && "did not find address base");

	TAG_argptr_t get_base =	TAG_get_fn(TAG_INT, TAG_FN_INT_GET);


	int64_t base = get_base.INT->get(tag);

	assert(base >= 0 && "device must have base > 0");

	return (uint64_t)base;
}

uint64_t CFG_get_address_size_node(CFG_node_t *node)
{
	TAG_tag_t *tag = getdata_from_hash_table(node->initals, "SIZE");
	assert(tag != NULL && "did not find address size");
	TAG_argptr_t get_size =	TAG_get_fn(TAG_INT, TAG_FN_INT_GET);

	int64_t size = get_size.INT->get(tag);

	assert(size > 0 && "device must have size > 0");

	return (uint64_t)size;
}

CFG_node_id_t CFG_has_pretag_node(CFG_node_t *node)
{
	TAG_tag_t *tag = getdata_from_hash_table(node->initals, "PRETAG");
	if(tag == NULL)
		return -1;
	TAG_argptr_t get_tag =	TAG_get_fn(TAG_INT, TAG_FN_INT_GET);

	int64_t dtag = get_tag.INT->get(tag);


	return (uint64_t)dtag;
}

//init route table
void CFG_init_routetable_node(CFG_node_t *node, uint64_t tags_size, CFG_node_id_t *ids, uint64_t edge_size)
{
	assert(tags_size > 1);
	assert(ids);
	node->routetable.tags_size = tags_size;
	node->done.init_routetable = true;

	CFG_node_route_row_t *rtags = calloc(tags_size, sizeof(CFG_node_route_row_t));
	assert(rtags);

	for(uint64_t i = 0; i < tags_size; ++i)
	{
		rtags[i].size = 0;
		CFG_node_route_t *routes = calloc(edge_size, sizeof(CFG_node_route_t));
		assert(routes);
		rtags[i].routes = routes;
		rtags[i].tag = ids[i];
	}

	node->routetable.tags = rtags;


	return;
}

void CFG_append_route_node(CFG_node_t *node, uint64_t index, CFG_node_route_t route)
{
	assert(node->done.init_routetable == true);
	CFG_node_route_row_t *tag = &node->routetable.tags[index];
	assert(tag->tag != node->pretag && "don't append a route to the devcfg row");
	tag->routes[tag->size++] = route;

}

void CFG_print_node(CFG_node_t *node)
{
	printf("node: %s\n",node->module);


	if(node->done.init_routetable)
	{
		for(uint64_t i = 0; i < node->routetable.tags_size; ++i)
		{
			CFG_node_route_row_t *row = &node->routetable.tags[i];

			for(uint64_t k = 0; k < row->size; ++k)
			{
				CFG_node_route_t *route = &row->routes[k];
				printf("route[%ld]: latency:%ld wireid:%ld index:%ld\n", k, route->latency, route->wire, route->latency);
			}
		}

	}
}