#include "SIM_prototag.h"
#include "TAG_bool.h"
#include "TAG_list.h"
#include "TAG_tag.h"
#include "TAG_int.h"
#include "TAG_string.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

SIM_prototag_t *SIM_init_prototag_empty(char *key)
{
	SIM_prototag_t *prototag = calloc(1, sizeof(SIM_prototag_t));
	assert(prototag);
	assert(key);
	char *dup = strdup(key);
	assert(dup);
	prototag->key = dup;
	prototag->tag = NULL;
	return prototag;
}

SIM_prototag_t *SIM_init_prototag_string(char *key, char *value)
{

	TAG_argptr_t argptr =	TAG_get_init(TAG_STRING, TAG_FN_STRING_INIT);

	TAG_ptr_t ptr = argptr.STRING->init(value);
	TAG_tag_t *tag = TAG_init(ptr, TAG_STRING);
	assert(tag);
	SIM_prototag_t *prototag = SIM_init_prototag_empty(key);
	assert(prototag);
	prototag->tag = tag;
	return prototag;
}

SIM_prototag_t *SIM_init_prototag_int(char *key, int64_t integer)
{

	TAG_argptr_t argptr =	TAG_get_init(TAG_INT, TAG_FN_INT_INIT);

	TAG_ptr_t ptr = argptr.INT->init(integer);
	TAG_tag_t *tag = TAG_init(ptr, TAG_INT);
	assert(tag);
	SIM_prototag_t *prototag = SIM_init_prototag_empty(key);
	assert(prototag);
	prototag->tag = tag;
	return prototag;
}

SIM_prototag_t *SIM_init_prototag_list_string(char *key, char **str, uint64_t count)
{
	TAG_argptr_t argptr =	TAG_get_init(TAG_LIST, TAG_FN_LIST_INIT_STRINGS);

	TAG_ptr_t ptr = argptr.LIST->init_strings(count, str);
	TAG_tag_t *tag = TAG_init(ptr, TAG_LIST);
	assert(tag);
	SIM_prototag_t *prototag = SIM_init_prototag_empty(key);
	assert(prototag);
	prototag->tag = tag;
	return prototag;
}

SIM_prototag_t *SIM_init_prototag_list(char *key, TAG_tag_t *tags, uint64_t count)
{

	assert(0);
}

SIM_prototag_t *SIM_init_prototag_bool(char *key, bool boolean)
{


	TAG_argptr_t argptr =	TAG_get_init(TAG_BOOL, TAG_FN_BOOL_INIT);

	TAG_ptr_t ptr = argptr.BOOL->init(boolean);
	TAG_tag_t *tag = TAG_init(ptr, TAG_BOOL);
	assert(tag);
	SIM_prototag_t *prototag = SIM_init_prototag_empty(key);
	assert(prototag);
	prototag->tag = tag;
	return prototag;
}

void SIM_free_prototag(SIM_prototag_t *prototag)
{
	printf("freeing tag\n");
	free(prototag->key);
	free(prototag);

}