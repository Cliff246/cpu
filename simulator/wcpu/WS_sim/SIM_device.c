#include "SIM_device.h"
#include "CFG_entry.h"
#include "CFG_prototag.h"
#include "SIM_channel.h"
#include "TAG_int.h"
#include "TAG_list.h"
#include "dynamic_lib.h"
#include "hashmap.h"

#include "TAG_tag.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void SIM_free_tag_table_elem(void *v)
{
	TAG_tag_t *tag = (TAG_tag_t *)v;

	TAG_free(tag);
}


bool SIM_prototag_add_to_device(SIM_device_t *device, CFG_prototag_t *tag)
{
	//key should be coppied
	addto_hash_table(device->tags, tag->key, tag->tag);
	CFG_free_prototag(tag);
	return true;
}

static void SIM_device_create_channels(SIM_device_t *device)
{
	TAG_tag_t *tag = getdata_from_hash_table(device->tags, "WIRE");
	assert(tag && "device must contain a wire tag");
	TAG_argptr_t get_size_arg =	TAG_get_fn(TAG_LIST, TAG_FN_LIST_GET_SIZE);
	TAG_argptr_t get_arg = TAG_get_fn(TAG_LIST, TAG_FN_LIST_GET);
	const uint64_t list_size = get_size_arg.LIST->get_size(tag);


	TAG_argptr_t get_int = TAG_get_fn(TAG_INT, TAG_FN_INT_GET);

	SIM_channel_t *channels = calloc(list_size, sizeof(SIM_channel_t));
	assert(channels);

	device->size = list_size;

	for(uint64_t i = 0; i < list_size; ++i)
	{

		TAG_tag_t *tmp =  get_arg.LIST->get(tag, i);
		assert(tmp->type == TAG_INT);
		int64_t wire = get_int.INT->get(tmp);
		channels[i].id = wire;
		channels[i].device = device;
		SIM_channel_print(&channels[i]);
	}
	device->channels = channels;

}

static void SIM_device_create_tags(SIM_device_t *device, CFG_entry_t *entry)
{
	const uint64_t count = entry->size;
	const uint32_t standard = 100;
	if(count >= standard)
	{
		assert(0);
	}
	p_hashtable_t table = new_hash_table(standard, SIM_free_tag_table_elem);
	assert(table);


	device->tags = table;
	WS_dynamic_lib_t *lib = WS_dynamic_lib_get(entry->module);
	assert(lib);
	device->mod = lib->dev_desc;
	for(uint32_t i = 0; i < count; ++i)
	{
		CFG_prototag_t *proto = entry->prototags[i];
		assert(SIM_prototag_add_to_device(device, proto) == true);
	}
	CFG_free_entry(entry);
}

void SIM_init_device(SIM_device_t *device, CFG_entry_t *entry)
{
	assert(device);


	SIM_device_create_tags(device, entry);
	SIM_device_create_channels(device);





}


SIM_channel_t *SIM_device_get_channel_by_wireid(SIM_device_t *device, uint64_t id)
{
	for(int i =0 ; i < device->size; ++i)
	{
		if(device->channels[i].id == id)
		{
			return &device->channels[i];
		}
	}
	return NULL;
}

void SIM_device_contains_connector(SIM_device_t *device, int64_t *id, uint64_t size)
{
	assert(device);
	assert(device->tags);
	TAG_tag_t *tag = getdata_from_hash_table(device->tags, "WIRE");
	assert(tag && "device must contain a wire tag");
	TAG_argptr_t get_size_arg =	TAG_get_fn(TAG_LIST, TAG_FN_LIST_GET_SIZE);
	TAG_argptr_t get_arg = TAG_get_fn(TAG_LIST, TAG_FN_LIST_GET);
	const uint64_t list_size = get_size_arg.LIST->get_size(tag);


	TAG_argptr_t get_int = TAG_get_fn(TAG_INT, TAG_FN_INT_GET);

	uint64_t has[list_size];

	for(uint64_t i = 0; i < list_size; ++i)
	{

		TAG_tag_t *tmp =  get_arg.LIST->get(tag, i);
		assert(tmp->type == TAG_INT);
		int64_t wire = get_int.INT->get(tmp);
		has[i] = wire;
	}

	memset(id, 0, sizeof(int64_t) * size);

	for(uint64_t k = 0; k < list_size; ++k)
	{
		id[has[k]] = INT64_MAX;
	}


}

uint64_t SIM_device_get_connectors(SIM_device_t *device, int64_t *buf, uint64_t size)
{
	assert(device);
	assert(device->tags);
	TAG_tag_t *tag = getdata_from_hash_table(device->tags, "WIRE");
	assert(tag && "device must contain a wire tag");
	TAG_argptr_t get_size_arg =	TAG_get_fn(TAG_LIST, TAG_FN_LIST_GET_SIZE);
	TAG_argptr_t get_arg = TAG_get_fn(TAG_LIST, TAG_FN_LIST_GET);
	const uint64_t list_size = get_size_arg.LIST->get_size(tag);
	//printf("%d %d\n", list_size, size);
	assert(list_size <= size);

	TAG_argptr_t get_int = TAG_get_fn(TAG_INT, TAG_FN_INT_GET);
	for(uint64_t i = 0; i < list_size; ++i)
	{

		TAG_tag_t *tmp =  get_arg.LIST->get(tag, i);
		assert(tmp->type == TAG_INT);
		int64_t wire = get_int.INT->get(tmp);
		buf[i] = wire;
	}
	return list_size;
}


void SIM_device_print(SIM_device_t *device)
{

}