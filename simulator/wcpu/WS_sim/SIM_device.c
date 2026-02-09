#include "SIM_device.h"
#include "CFG_entry.h"
#include "CFG_prototag.h"
#include "SIM_channel.h"
#include "SIM_chnlcfg.h"
#include "TAG_int.h"
#include "TAG_list.h"
#include "dynamic_lib.h"
#include "hashmap.h"

#include "TAG_tag.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>



p_hashtable_t SIM_copy_hashtable(p_hashtable_t old)
{
 	p_hashtable_t copy = new_hash_table(100, SIM_free_tag_table_elem);


	reset_iter_hashtable(old);
	p_hashelem_t elem = next_iter_hashtable(old);
	while(elem != NULL)
	{
		TAG_tag_t *tagcopy = TAG_copy(elem->p_data);
		TAG_print(tagcopy);
		addto_hash_table(copy, elem->p_key, tagcopy);
		elem = next_iter_hashtable(old);

	}
	return copy;
}

void SIM_init_device(SIM_device_t *device, SIM_devcfg_t *devctx)
{
	assert(device);

	device->tags = SIM_copy_hashtable(devctx->initals);


	print_hash_table(device->tags);
	printf("\n");
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


SIM_dtag_t SIM_device_get_dtag(SIM_device_t *device)
{
	return device->dtag;
}

bool SIM_device_has_address_range(SIM_device_t *device)
{

	TAG_tag_t *tag1 = getdata_from_hash_table(device->tags, "ADRRESS_BASE");
	if(tag1 == NULL)
		return false;
	TAG_argptr_t get_base =	TAG_get_fn(TAG_INT, TAG_FN_INT_GET);

	TAG_tag_t *tag2 = getdata_from_hash_table(device->tags, "ADDRESS_SIZE");
	if(tag2 == NULL)
		return false;
	TAG_argptr_t get_size =	TAG_get_fn(TAG_INT, TAG_FN_INT_GET);

	return true;
}

uint64_t SIM_device_get_address_base(SIM_device_t *device)
{

	TAG_tag_t *tag = getdata_from_hash_table(device->tags, "ADRRESS_BASE");
	assert(tag != NULL && "did not find address base");

	TAG_argptr_t get_base =	TAG_get_fn(TAG_INT, TAG_FN_INT_GET);


	int64_t base = get_base.INT->get(tag);

	assert(base > 0 && "device must have base > 0");

	return (uint64_t)base;
}

uint64_t SIM_device_get_address_size(SIM_device_t *device)
{


	TAG_tag_t *tag = getdata_from_hash_table(device->tags, "ADDRESS_SIZE");
	assert(tag != NULL && "did not find address size");
	TAG_argptr_t get_size =	TAG_get_fn(TAG_INT, TAG_FN_INT_GET);

	int64_t size = get_size.INT->get(tag);

	assert(size > 0 && "device must have size > 0");

	return (uint64_t)size;
}