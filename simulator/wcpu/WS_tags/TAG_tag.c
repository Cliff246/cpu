#include "TAG_tag.h"


#include "TAG_string.h"
#include "TAG_int.h"
#include "TAG_bool.h"
#include "TAG_list.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


#define TAG_VTABLE_FILL(X, Y, Z) [WS_SIM_TAG_ENUM_NAME(Y)] = &X##_vtable ,
#define TAG_TYPE_FILL(X, Y, Z) [WS_SIM_TAG_ENUM_NAME(Y)] = #Y,

TAG_prototype_vtable_t *TAG_vtable_list[WS_SIM_TAG_LIST_COUNT] =
{
	WS_SIM_TAG_LIST(TAG_VTABLE_FILL)
};

char *TAG_type_string_list[WS_SIM_TAG_LIST_COUNT] =
{
	WS_SIM_TAG_LIST(TAG_TYPE_FILL)
};

void TAG_free(TAG_tag_t *tag)
{
	TAG_ptr_t ptr = tag->ptr;
	TAG_vtable_list[tag->type]->free(ptr);
	free(tag);
}


TAG_argptr_t TAG_get_fn(TAG_type_t type, uint64_t index)
{
	if(TAG_vtable_list[type]->size <= index)
	{
		assert(0);
	}

	TAG_argptr_t argptr = TAG_vtable_list[type]->fn[index];
	return argptr;
}

TAG_tag_t *TAG_init(TAG_ptr_t ptr, TAG_type_t type)
{
	assert(ptr.SELF != NULL);
	TAG_tag_t *tag = calloc(1, sizeof(TAG_tag_t));
	assert(tag);
	tag->ptr = ptr;
	tag->type = type;
	return tag;
}

void TAG_print(TAG_tag_t *tag)
{
	printf("TAG<%s>: ", TAG_type_string_list[tag->type]);
	TAG_vtable_list[tag->type]->print(tag->ptr);
}