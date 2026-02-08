#include "TAG_list.h"
#include "TAG_int.h"
#include "TAG_string.h"
#include "TAG_tag.h"
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

TAG_list_arg_t init_list_bools;


TAG_list_arg_t init_list_strings;

TAG_list_arg_t init_list_ints;
TAG_list_arg_t init_list_tags;

TAG_list_arg_t get_list;

TAG_list_arg_t get_list_size;


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


void TAG_list_print(TAG_ptr_t ptr)
{
	printf("list: %lu\n", ptr.LIST->size);
	for(int i = 0; i < ptr.LIST->size; ++i)
	{
		TAG_print(ptr.LIST->list[i]);
	}
}

TAG_ptr_t TAG_list_copy(TAG_ptr_t ptr)
{
	TAG_list_t *old = ptr.LIST;
	uint64_t size = ptr.LIST->size;

	TAG_tag_t *tags[size];

	for(uint64_t i = 0; i < size; ++i)
	{
		TAG_tag_t *tocpy = old->list[i];
		TAG_tag_t *copied = TAG_copy(tocpy);
		tags[i] = copied;
	}

	TAG_ptr_t new = TAG_init_list_tags(size, tags);
	return new;
}


static TAG_list_t *TAG_init_list(uint64_t count)
{
	TAG_list_t *list = calloc(1, sizeof(TAG_list_t));
	assert(list);

	TAG_tag_t **contents = calloc(count, sizeof(TAG_tag_t *));
	assert(contents);
	list->list = contents;
	list->size = count;

	return list;
}

static TAG_ptr_t TAG_init_list_bools(uint64_t size, bool *bools)
{
	assert(0);
}
static TAG_ptr_t TAG_init_list_strings(uint64_t size, char **strings)
{
	TAG_list_t *list = TAG_init_list(size);
	TAG_argptr_t string_init = TAG_get_fn(TAG_STRING, TAG_FN_STRING_INIT);
	for(uint64_t i = 0; i < size; ++i)
	{
		TAG_ptr_t tptr = string_init.STRING->init(strings[i]);
		TAG_tag_t *ttag = TAG_init(tptr, TAG_STRING);
		list->list[i] = ttag;
	}
	TAG_ptr_t ptr;
	ptr.LIST = list;
	return ptr;
}



static TAG_ptr_t TAG_init_list_ints(uint64_t size, int64_t *integer)
{
	TAG_list_t *list = TAG_init_list(size);
	TAG_argptr_t int_init = TAG_get_fn(TAG_INT, TAG_FN_INT_INIT);
	for(uint64_t i = 0; i < size; ++i)
	{
		TAG_ptr_t tptr = int_init.INT->init(integer[i]);
		TAG_tag_t *ttag = TAG_init(tptr, TAG_INT);
		list->list[i] = ttag;
	}
	TAG_ptr_t ptr;
	ptr.LIST = list;
	return ptr;

}

static TAG_ptr_t TAG_init_list_tags(uint64_t size, TAG_tag_t **tags)
{
	TAG_list_t *list = TAG_init_list(size);
	assert(list);
	for(uint64_t i = 0; i < size; ++i)
	{
		list->list[i] = tags[i];
	}


	TAG_ptr_t ptr;
	ptr.LIST = list;

	return ptr;
}

static TAG_tag_t *TAG_get_list(TAG_tag_t *tag, uint64_t pos)
{
	if(tag->ptr.LIST->size < pos)
		return NULL;
	return tag->ptr.LIST->list[pos];
}

static uint64_t TAG_get_list_size(TAG_tag_t *tag)
{
	return tag->ptr.LIST->size;
}


TAG_list_arg_t init_list_bools =
{
	.init_bools = TAG_init_list_bools,
};



TAG_list_arg_t init_list_strings =
{
	.init_strings = TAG_init_list_strings
};

TAG_list_arg_t init_list_ints =
{
	.init_ints = TAG_init_list_ints
};

TAG_list_arg_t init_list_tags =
{
	.init_tags = TAG_init_list_tags
};

TAG_list_arg_t get_list =
{
	.get = TAG_get_list
};

TAG_list_arg_t get_list_size =
{
	.get_size = TAG_get_list_size
};



TAG_prototype_vtable_t TAG_list_vtable =
{
	.free = TAG_list_free,
	.print = TAG_list_print,
	.copy = TAG_list_copy,
	.size = 6,
	.fn =
	{
		[TAG_FN_LIST_INIT_BOOLS].LIST = &init_list_bools,
		[TAG_FN_LIST_INIT_STRINGS].LIST = &init_list_strings,
		[TAG_FN_LIST_INIT_INTS].LIST = &init_list_ints,
		[TAG_FN_LIST_INIT_TAGS].LIST = &init_list_tags,
		[TAG_FN_LIST_GET].LIST = &get_list,
		[TAG_FN_LIST_GET_SIZE].LIST = &get_list_size,
	}

};



