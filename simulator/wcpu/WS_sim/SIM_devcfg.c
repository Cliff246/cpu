#include "SIM_devcfg.h"
#include "SIM_chnlcfg.h"
#include "SIM_commons.h"
#include "SIM_handle.h"
#include "TAG_tag.h"
#include "TAG_int.h"
#include "TAG_list.h"
#include "TAG_string.h"
#include "TAG_bool.h"
#include <stdbool.h>
#include <stdalign.h>
#include <stdint.h>
#include <assert.h>

bool SIM_devcfg_add_prototag(SIM_devcfg_t *ptr, CFG_prototag_t *tag)
{
	//key should be coppied
	addto_hash_table(ptr->initals, tag->key, tag->tag);
	CFG_free_prototag(tag);
	return true;
}

bool SIM_init_devcfg_tags(SIM_devcfg_t *ptr, CFG_entry_t *entry)
{
	const uint64_t count = entry->size;
	const uint32_t standard = 100;
	if(count >= standard)
	{
		assert(0);
	}
	p_hashtable_t table = new_hash_table(standard, SIM_free_tag_table_elem);
	assert(table);


	ptr->initals = table;
	SIM_handle_t *handle = SIM_init_handle(entry->module);
	assert(handle);
	ptr->handle = handle;
	for(uint32_t i = 0; i < count; ++i)
	{
		CFG_prototag_t *proto = entry->prototags[i];
		assert(SIM_devcfg_add_prototag(ptr, proto) == true);
	}
	CFG_free_entry(entry);
	return true;
}

bool SIM_devcfg_prep_chnlcfgs(SIM_devcfg_t *ptr,  SIM_chnlcfg_buf_t *buf)
{
	TAG_tag_t *tag = getdata_from_hash_table(ptr->initals, "WIRE");
	assert(tag && "device must contain a wire tag");
	TAG_argptr_t get_size_arg =	TAG_get_fn(TAG_LIST, TAG_FN_LIST_GET_SIZE);
	TAG_argptr_t get_arg = TAG_get_fn(TAG_LIST, TAG_FN_LIST_GET);
	const uint64_t list_size = get_size_arg.LIST->get_size(tag);


	TAG_argptr_t get_int = TAG_get_fn(TAG_INT, TAG_FN_INT_GET);
	ptr->channels = list_size;

	SIM_chnlcfg_t configs[list_size];


	for(uint64_t i = 0; i < list_size; ++i)
	{

		TAG_tag_t *tmp =  get_arg.LIST->get(tag, i);
		assert(tmp->type == TAG_INT);
		int64_t wire = get_int.INT->get(tmp);
		configs[i].wireid = wire;
		configs[i].devcfg = ptr;
	}
	SIM_append_chnlcfg_buf(buf, configs, list_size);

	return true;
}

bool SIM_init_devcfg(SIM_devcfg_t *ptr, CFG_entry_t *entry, SIM_chnlcfg_buf_t *buf)
{

	SIM_init_devcfg_tags(ptr, entry);
	SIM_devcfg_prep_chnlcfgs(ptr, buf);

	return true;
}