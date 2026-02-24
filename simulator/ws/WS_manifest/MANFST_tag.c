#include "MANFST_tag.h"
#include "TAG_bool.h"
#include "TAG_list.h"
#include "TAG_map.h"
#include "TAG_tag.h"
#include "TAG_int.h"
#include "TAG_string.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

TAG_tag_t *MANFST_init_tag_string(char *value)
{

	TAG_argptr_t argptr =	TAG_get_fn(TAG_STRING, TAG_FN_STRING_INIT);

	TAG_ptr_t ptr = argptr.STRING->init(value);
	TAG_tag_t *tag = TAG_init(ptr, TAG_STRING);
	assert(tag);
	return tag;
}
TAG_tag_t *MANFST_init_tag_int(int64_t integer)
{

	TAG_argptr_t argptr =	TAG_get_fn(TAG_INT, TAG_FN_INT_INIT);

	TAG_ptr_t ptr = argptr.INT->init(integer);
	TAG_tag_t *tag = TAG_init(ptr, TAG_INT);
	assert(tag);
	return tag;
}
TAG_tag_t *MANFST_init_tag_list_string(char **str, uint64_t count)
{
	TAG_argptr_t argptr =	TAG_get_fn(TAG_LIST, TAG_FN_LIST_INIT_STRINGS);

	TAG_ptr_t ptr = argptr.LIST->init_strings(count, str);
	TAG_tag_t *tag = TAG_init(ptr, TAG_LIST);
	return tag;
}
TAG_tag_t *MANFST_init_tag_list_ints(int64_t *integers, uint64_t count)
{
	TAG_argptr_t argptr =	TAG_get_fn(TAG_LIST, TAG_FN_LIST_INIT_INTS);

	TAG_ptr_t ptr = argptr.LIST->init_ints(count, integers);
	TAG_tag_t *tag = TAG_init(ptr, TAG_LIST);
	return tag;

}
TAG_tag_t *MANFST_init_tag_list_bools(bool *booleans, uint64_t count)
{
	TAG_argptr_t argptr =	TAG_get_fn(TAG_LIST, TAG_FN_LIST_INIT_BOOLS);

	TAG_ptr_t ptr = argptr.LIST->init_bools(count, booleans);
	TAG_tag_t *tag = TAG_init(ptr, TAG_LIST);
	return tag;
}
TAG_tag_t *MANFST_init_tag_list()
{
	TAG_argptr_t argptr =	TAG_get_fn(TAG_LIST, TAG_FN_LIST_INIT_EMPTY);

	TAG_ptr_t ptr = argptr.LIST->init_empty();
	TAG_tag_t *tag = TAG_init(ptr, TAG_LIST);
	return tag;
}
TAG_tag_t *MANFST_init_tag_map()
{
	TAG_argptr_t argptr =	TAG_get_fn(TAG_MAP, TAG_FN_MAP_INIT);

	TAG_ptr_t ptr = argptr.MAP->init();
	TAG_tag_t *tag = TAG_init(ptr, TAG_MAP);
	return tag;
}

TAG_tag_t *MANFST_init_tag_bool(bool boolean)
{


	TAG_argptr_t argptr =	TAG_get_fn(TAG_BOOL, TAG_FN_BOOL_INIT);

	TAG_ptr_t ptr = argptr.BOOL->init(boolean);
	TAG_tag_t *tag = TAG_init(ptr, TAG_BOOL);
	assert(tag);
	return tag;
}