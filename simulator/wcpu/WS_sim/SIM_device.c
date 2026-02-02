#include "SIM_device.h"
#include "CFG_prototag.h"
#include "hashmap.h"

#include "TAG_tag.h"
#include <assert.h>
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

SIM_device_t *SIM_init_device(CFG_prototag_t **tags, uint64_t count)
{


	const uint32_t standard = 100;
	if(count >= standard)
	{
		assert(0);
	}
	p_hashtable_t table = new_hash_table(standard, SIM_free_tag_table_elem);
	assert(table);

	SIM_device_t *device = calloc(1, sizeof(SIM_device_t));
	device->tags = table;


	for(uint32_t i = 0; i < count; ++i)
	{
		assert(SIM_prototag_add_to_device(device, tags[i]) == true);
	}
	return device;
}