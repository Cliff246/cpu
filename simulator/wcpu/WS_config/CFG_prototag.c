#include "CFG_prototag.h"
#include "TAG_bool.h"
#include "TAG_list.h"
#include "TAG_tag.h"
#include "TAG_int.h"
#include "TAG_string.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

CFG_prototag_t *CFG_init_prototag_empty(char *key)
{
	CFG_prototag_t *prototag = calloc(1, sizeof(CFG_prototag_t));
	assert(prototag);
	assert(key);
	char *dup = strdup(key);
	assert(dup);
	prototag->key = dup;
	prototag->tag = NULL;
	return prototag;
}

CFG_prototag_t *CFG_init_prototag_string(char *key, char *value)
{

	TAG_argptr_t argptr =	TAG_get_fn(TAG_STRING, TAG_FN_STRING_INIT);

	TAG_ptr_t ptr = argptr.STRING->init(value);
	TAG_tag_t *tag = TAG_init(ptr, TAG_STRING);
	assert(tag);
	CFG_prototag_t *prototag = CFG_init_prototag_empty(key);
	assert(prototag);
	prototag->tag = tag;
	return prototag;
}

CFG_prototag_t *CFG_init_prototag_int(char *key, int64_t integer)
{

	TAG_argptr_t argptr =	TAG_get_fn(TAG_INT, TAG_FN_INT_INIT);

	TAG_ptr_t ptr = argptr.INT->init(integer);
	TAG_tag_t *tag = TAG_init(ptr, TAG_INT);
	assert(tag);
	CFG_prototag_t *prototag = CFG_init_prototag_empty(key);
	assert(prototag);
	prototag->tag = tag;
	return prototag;
}

CFG_prototag_t *CFG_init_prototag_list_string(char *key, char **str, uint64_t count)
{
	TAG_argptr_t argptr =	TAG_get_fn(TAG_LIST, TAG_FN_LIST_INIT_STRINGS);

	TAG_ptr_t ptr = argptr.LIST->init_strings(count, str);
	TAG_tag_t *tag = TAG_init(ptr, TAG_LIST);
	assert(tag);
	CFG_prototag_t *prototag = CFG_init_prototag_empty(key);
	assert(prototag);
	prototag->tag = tag;
	return prototag;
}

CFG_prototag_t *CFG_init_prototag_list_ints(char *key, int64_t *integers, uint64_t count)
{
	TAG_argptr_t argptr =	TAG_get_fn(TAG_LIST, TAG_FN_LIST_INIT_INTS);

	TAG_ptr_t ptr = argptr.LIST->init_ints(count, integers);
	TAG_tag_t *tag = TAG_init(ptr, TAG_LIST);
	assert(tag);
	CFG_prototag_t *prototag = CFG_init_prototag_empty(key);
	assert(prototag);
	prototag->tag = tag;
	return prototag;
}

CFG_prototag_t *CFG_init_prototag_list_bools(char *key, bool *booleans, uint64_t count)
{
	TAG_argptr_t argptr =	TAG_get_fn(TAG_LIST, TAG_FN_LIST_INIT_BOOLS);

	TAG_ptr_t ptr = argptr.LIST->init_bools(count, booleans);
	TAG_tag_t *tag = TAG_init(ptr, TAG_LIST);
	assert(tag);
	CFG_prototag_t *prototag = CFG_init_prototag_empty(key);
	assert(prototag);
	prototag->tag = tag;
	return prototag;
}

CFG_prototag_t *CFG_init_prototag_list(char *key, TAG_tag_t *tags, uint64_t count)
{

	assert(0);
}

CFG_prototag_t *CFG_init_prototag_bool(char *key, bool boolean)
{


	TAG_argptr_t argptr =	TAG_get_fn(TAG_BOOL, TAG_FN_BOOL_INIT);

	TAG_ptr_t ptr = argptr.BOOL->init(boolean);
	TAG_tag_t *tag = TAG_init(ptr, TAG_BOOL);
	assert(tag);
	CFG_prototag_t *prototag = CFG_init_prototag_empty(key);
	assert(prototag);
	prototag->tag = tag;
	return prototag;
}

void CFG_free_prototag(CFG_prototag_t *prototag)
{
	free(prototag->key);
	free(prototag);

}

void CFG_print_prototag(CFG_prototag_t *prototag)
{
	printf("prototag: %s\n", prototag->key);
	TAG_print(prototag->tag);
}