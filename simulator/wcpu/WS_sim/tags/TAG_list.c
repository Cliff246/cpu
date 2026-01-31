#include "TAG_list.h"
#include "TAG_tag.h"
#include <stdint.h>
#include <assert.h>




void TAG_list_free(TAG_ptr_t ptr)
{
	TAG_list_t *list = ptr.LIST;
	for(uint64_t i = 0; i < list->size; ++i)
	{
		TAG_tag_t *tag = list->list[i];
		//don't self free...
		assert(ptr.SELF != tag->ptr.SELF && "Cannot self free a list in a list");

		TAG_free(tag);
	}
	free(list->list);
	free(list);
}



static TAG_ptr_t TAG_init_list_bools(uint64_t size, bool *bools)
{
	assert(0);
}
static TAG_ptr_t TAG_init_list_strings(uint64_t size, char **strings)
{
	assert(0);

}
static TAG_ptr_t TAG_init_list_ints(uint64_t size, int64_t *integer)
{
	assert(0);

}

static TAG_list_arg_t init_list_bools =
{
	.init_bools = TAG_init_list_bools,
};



static TAG_list_arg_t init_list_strings =
{
	.init_strings = TAG_init_list_strings
};

static TAG_list_arg_t init_list_int =
{
	.init_ints = TAG_init_list_ints
};


TAG_prototype_vtable_t TAG_list_vtable =
{
	.free = TAG_list_free,
	.size = 3,
	.fn =
	{
		[TAG_FN_LIST_INIT_BOOLS].LIST = &init_list_bools,
		[TAG_FN_LIST_INIT_STRINGS].LIST = &init_list_strings,
		[TAG_FN_LIST_INIT_INTS].LIST = &init_list_int,
	}

};



