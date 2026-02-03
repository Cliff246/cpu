#include "SIM_device.h"
#include "CFG_entry.h"
#include "CFG_prototag.h"
#include "dynamic_lib.h"
#include "hashmap.h"

#include "TAG_tag.h"
#include <assert.h>
#include <stdint.h>
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

SIM_device_t *SIM_init_device(CFG_entry_t *entry)
{

	const uint64_t count = entry->size;
	const uint32_t standard = 100;
	if(count >= standard)
	{
		assert(0);
	}
	p_hashtable_t table = new_hash_table(standard, SIM_free_tag_table_elem);
	assert(table);

	SIM_device_t *device = calloc(1, sizeof(SIM_device_t));
	assert(device);
	device->tags = table;
	device->dl = WS_dynamic_lib_get(entry->module);
	assert(device->dl);
	for(uint32_t i = 0; i < count; ++i)
	{
		CFG_prototag_t *proto = entry->prototags[i];
		assert(SIM_prototag_add_to_device(device, proto) == true);
	}
	CFG_free_entry(entry);
	return device;
}