#include "SCENE_port.h"
#include "SCENE_commons.h"

#include "SCENE_link.h"
#include "SCENE_topology.h"
#include "TAG_int.h"
#include "TAG_list.h"
#include "TAG_string.h"
#include "TAG_tag.h"
#include "TAG_map.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//------------------------------------
//commons prototype
//------------------------------------


//-------------------------------------
//producers prototype
//------------------------------------
void SCENE_get_ints_port(SCENE_port_t *port, TAG_tag_t *tag);
void SCENE_get_channels_port(SCENE_port_t *port, TAG_tag_t *tag);
void SCENE_get_links_port(SCENE_port_t *port, TAG_tag_t *tag, SCENE_topology_t *topology);


//-----------------------------------
//commons impl
//-----------------------------------







//----------------------------------
//producers impl
//----------------------------------


void SCENE_get_ints_port(SCENE_port_t *port, TAG_tag_t *tag)
{
	TAG_argptr_t get_key = TAG_get_fn(TAG_MAP, TAG_FN_MAP_GET_KEY_STRING);
	TAG_argptr_t get_int = TAG_get_fn(TAG_INT, TAG_FN_INT_GET);
	TAG_argptr_t get_size = TAG_get_fn(TAG_MAP, TAG_FN_MAP_GET_SIZE);



	TAG_tag_t *lanes = get_key.MAP->get_key_string(tag, "lanes");
	TAG_tag_t *channels = get_key.MAP->get_key_string(tag, "channels");

	TAG_tag_t *buffer[10];

	int64_t lanes_count = SCENE_get_all_tags_map(lanes, buffer, 10);
	assert(lanes_count == 1);

	TAG_tag_t *lanes_int_tag = buffer[0];
	int64_t lanes_int_value = get_int.INT->get(lanes_int_tag);


	int64_t channels_count = SCENE_get_all_tags_map(channels, buffer, 10);
	assert(channels_count == 1);

	TAG_tag_t *channels_int_tag = buffer[0];
	int64_t channels_int_value = get_int.INT->get(channels_int_tag);





	port->lanes = lanes_int_value;
	port->channels_count = channels_int_value;



}
void SCENE_get_channels_port(SCENE_port_t *port, TAG_tag_t *tag)
{
	TAG_argptr_t get_key = TAG_get_fn(TAG_MAP, TAG_FN_MAP_GET_KEY_STRING);
	TAG_argptr_t get_int = TAG_get_fn(TAG_INT, TAG_FN_INT_GET);
	TAG_argptr_t get_size = TAG_get_fn(TAG_MAP, TAG_FN_MAP_GET_SIZE);

	TAG_argptr_t get_pos_list = TAG_get_fn(TAG_LIST, TAG_FN_LIST_GET);
	TAG_argptr_t get_size_list = TAG_get_fn(TAG_LIST, TAG_FN_LIST_GET_SIZE);

	TAG_argptr_t get_string = TAG_get_fn(TAG_STRING, TAG_FN_STRING_GET);

	TAG_tag_t *positive = get_key.MAP->get_key_string(tag, "channels_positive");
	TAG_tag_t *negative = get_key.MAP->get_key_string(tag, "channels_negative");


	TAG_tag_t *buffer[10];

	int64_t positive_count = SCENE_get_all_tags_map(positive, buffer, 10);
	assert(positive_count == 1);
	TAG_tag_t *positive_tag = buffer[0];


	uint64_t positive_list_count = get_size_list.LIST->get_size(positive_tag);

	char **positive_channels = calloc(positive_list_count, sizeof(char *));
	assert(positive_channels);

	for(uint64_t i = 0; i < positive_list_count; ++i)
	{

		char *str = get_string.STRING->get(	get_pos_list.LIST->get(positive_tag, i));
		positive_channels[i] = str;
	}

	port->positive = positive_channels;

	int64_t negative_count = SCENE_get_all_tags_map(negative, buffer, 10);
	assert(negative_count == 1);

	TAG_tag_t *negative_tag = buffer[0];

	uint64_t negative_list_count = get_size_list.LIST->get_size(negative_tag);

	char **negative_channels = calloc(negative_list_count, sizeof(char *));
	assert(negative_channels);


	for(uint64_t i = 0; i < negative_list_count; ++i)
	{

		char *str = get_string.STRING->get(	get_pos_list.LIST->get(negative_tag, i));
		negative_channels[i] = str;
	}

	port->negative = negative_channels;

	assert(negative_list_count == positive_list_count && "postive must match negatives but this is the basic test");
	assert(positive_list_count == negative_list_count &&
       "channels_positive and channels_negative must match");

	assert(positive_list_count == port->channels_count &&
       "channel count must match list size");
}

void SCENE_get_links_port(SCENE_port_t *port, TAG_tag_t *tag, SCENE_topology_t *topology)
{
	TAG_argptr_t get_key = TAG_get_fn(TAG_MAP, TAG_FN_MAP_GET_KEY_STRING);
	TAG_argptr_t get_int = TAG_get_fn(TAG_INT, TAG_FN_INT_GET);
	TAG_argptr_t get_size = TAG_get_fn(TAG_MAP, TAG_FN_MAP_GET_SIZE);

	TAG_argptr_t get_pos_list = TAG_get_fn(TAG_LIST, TAG_FN_LIST_GET);
	TAG_argptr_t get_size_list = TAG_get_fn(TAG_LIST, TAG_FN_LIST_GET_SIZE);

	TAG_argptr_t get_string = TAG_get_fn(TAG_STRING, TAG_FN_STRING_GET);

	TAG_tag_t *links_map = get_key.MAP->get_key_string(tag, "links");
	int64_t links_map_count = SCENE_get_size_tags_map(links_map);
	assert(links_map_count == 1);
	TAG_tag_t *buffer1[links_map_count];

	assert(SCENE_get_all_tags_map(links_map, buffer1, links_map_count) == links_map_count);


	TAG_tag_t *links = buffer1[0];
	uint64_t links_count =	SCENE_get_size_tags_map(links);

	TAG_tag_t *links_buf[links_count];

	assert(SCENE_get_all_tags_map(links, links_buf, links_count) == links_count);

	TAG_tag_t *links_elm_map = links_buf[0];
	//TAG_print(links_elm_map);
	uint64_t links_elm_count =	SCENE_get_size_tags_map(links_elm_map);
	assert(links_elm_count > 0);

	TAG_tag_t *links_elm[links_elm_count];
	assert(SCENE_get_all_tags_map(links_elm_map, links_elm, links_elm_count) == links_elm_count);
	uint64_t *links_locations = calloc(links_elm_count, sizeof(uint64_t));
	assert(links_locations);
	//printf("links_elm_count %ld\n", links_elm_count);

	for(uint64_t i = 0; i < links_elm_count; ++i)
	{
		//TAG_print(links_elm[i]);
		assert(links_elm[i] != NULL);
		uint64_t elm_list_size = get_size_list.LIST->get_size(links_elm[i]);
		assert(elm_list_size == 2);

		TAG_tag_t *pos_tag = get_pos_list.LIST->get(links_elm[i], 0);
		TAG_tag_t *neg_tag = get_pos_list.LIST->get(links_elm[i], 1);

		assert(pos_tag->type == TAG_STRING && neg_tag->type == TAG_STRING && "both positive and negative links must be strings");

		char *pos = get_string.STRING->get(pos_tag);

		char *neg = get_string.STRING->get(neg_tag);

		SCENE_link_t *link = SCENE_init_link(port->uid,pos, neg);

		links_locations[i] = SCENE_append_link_topology(topology, link);

		assert(pos && neg);

	}
	port->links = links_locations;
	port->links_count = links_elm_count;

}


//returns index into topology
uint64_t SCENE_init_port(TAG_tag_t *port_map, SCENE_topology_t *topology, SIM_uid_t uid)
{
	SCENE_port_t *port = calloc(1, sizeof(SCENE_port_t));

	static uint64_t port_id_counter = 0;

	port->port_id = port_id_counter++;
	port->uid = uid;

	SCENE_get_ints_port(port, port_map);
	SCENE_get_channels_port(port, port_map);
	SCENE_get_links_port(port, port_map, topology);

	uint64_t channel_count = port->channels_count;
	uint64_t link_count = port->links_count;
	assert(channel_count == link_count);
	//SCENE_print_port(port);
	uint64_t pos = SCENE_append_port_topology(topology, port);
	return pos;
}

void SCENE_print_port(SCENE_port_t *port)
{
	printf("port: lanes %ld channels %ld links_size %ld\n", port->lanes, port->channels_count, port->links_count);
	for(uint64_t i = 0; i < port->channels_count; ++i)
	{
		printf("+<%s>\n", port->positive[i]);
	}
	for(uint64_t j = 0; j < port->channels_count; ++j)
	{
		printf("-<%s>\n", port->negative[j]);
	}

	for(uint64_t k = 0; k < port->links_count; ++k)
	{
		printf("[%ld] = %ld\n", k, port->links[k]);
	}
}

void SCENE_free_port(SCENE_port_t *port)
{
	assert(0);
}