#include "MANFST_prototag.h"
#include "TAG_bool.h"
#include "TAG_list.h"
#include "TAG_tag.h"
#include "TAG_int.h"
#include "TAG_string.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>

//static functions
char *combine_strings_alloc(char *str1, uint64_t len1, char *str2, uint64_t len2);





//header functions
void MANFST_get_prototag_key(MANFST_prototag_t *prototag, char *buffer, uint64_t length)
{
	assert(prototag->key_size + 1 == length && "to get prototag_key must return a buffer of key_size + 1");
	uint64_t c = 0;
	for(uint64_t i = 0; i < prototag->key_size; ++i, ++c)
	{
		buffer[i] = prototag->buffer[c];
	}
	buffer[prototag->key_size] = 0;
	return;
}
void MANFST_get_prototag_name(MANFST_prototag_t *prototag, char *buffer, uint64_t length)

{
	assert(prototag->name_size + 1 == length && "to get prototag_name must return a buffer of name_size + 1");

	assert(prototag->name_size != 0 && "prototag cannot have name_size of zero and be called");
	uint64_t c = prototag->key_size;
	for(uint64_t i = 0; i < prototag->name_size; ++i, ++c)
	{
		buffer[i] = prototag->buffer[c];
	}
	buffer[prototag->name_size] = 0;
	return;
}

MANFST_prototag_t *MANFST_init_prototag_empty(char *key, char *name)
{
	MANFST_prototag_t *prototag = calloc(1, sizeof(MANFST_prototag_t));
	assert(prototag);
	assert(key);

	uint64_t key_size = strlen(key);
	uint64_t name_size = 0;

	if(name != NULL)
	{
		name_size = strlen(name);
	}

	char *buffer = combine_strings_alloc(key, key_size, name, name_size);

	prototag->name_size = name_size;
	prototag->key_size = key_size;
	prototag->buffer = buffer;



	prototag->tag = NULL;
	return prototag;
}

MANFST_prototag_t *MANFST_init_prototag_string(char *key, char *name, char *value)
{

	TAG_argptr_t argptr =	TAG_get_fn(TAG_STRING, TAG_FN_STRING_INIT);

	TAG_ptr_t ptr = argptr.STRING->init(value);
	TAG_tag_t *tag = TAG_init(ptr, TAG_STRING);
	assert(tag);
	MANFST_prototag_t *prototag = MANFST_init_prototag_empty(key, name);
	assert(prototag);
	prototag->tag = tag;
	return prototag;
}

MANFST_prototag_t *MANFST_init_prototag_int(char *key, char *name, int64_t integer)
{

	TAG_argptr_t argptr =	TAG_get_fn(TAG_INT, TAG_FN_INT_INIT);

	TAG_ptr_t ptr = argptr.INT->init(integer);
	TAG_tag_t *tag = TAG_init(ptr, TAG_INT);
	assert(tag);
	MANFST_prototag_t *prototag = MANFST_init_prototag_empty(key, name);
	assert(prototag);
	prototag->tag = tag;
	return prototag;
}

MANFST_prototag_t *MANFST_init_prototag_list_string(char *key, char *name, char **str, uint64_t count)
{
	TAG_argptr_t argptr =	TAG_get_fn(TAG_LIST, TAG_FN_LIST_INIT_STRINGS);

	TAG_ptr_t ptr = argptr.LIST->init_strings(count, str);
	TAG_tag_t *tag = TAG_init(ptr, TAG_LIST);
	assert(tag);
	MANFST_prototag_t *prototag = MANFST_init_prototag_empty(key, name);
	assert(prototag);
	prototag->tag = tag;
	return prototag;
}

MANFST_prototag_t *MANFST_init_prototag_list_ints(char *key, char *name, int64_t *integers, uint64_t count)
{
	TAG_argptr_t argptr =	TAG_get_fn(TAG_LIST, TAG_FN_LIST_INIT_INTS);

	TAG_ptr_t ptr = argptr.LIST->init_ints(count, integers);
	TAG_tag_t *tag = TAG_init(ptr, TAG_LIST);
	assert(tag);
	MANFST_prototag_t *prototag = MANFST_init_prototag_empty(key, name);
	assert(prototag);
	prototag->tag = tag;
	return prototag;
}

MANFST_prototag_t *MANFST_init_prototag_list_bools(char *key, char *name, bool *booleans, uint64_t count)
{
	TAG_argptr_t argptr =	TAG_get_fn(TAG_LIST, TAG_FN_LIST_INIT_BOOLS);

	TAG_ptr_t ptr = argptr.LIST->init_bools(count, booleans);
	TAG_tag_t *tag = TAG_init(ptr, TAG_LIST);
	assert(tag);
	MANFST_prototag_t *prototag = MANFST_init_prototag_empty(key, name);
	assert(prototag);
	prototag->tag = tag;
	return prototag;
}

MANFST_prototag_t *MANFST_init_prototag_list(char *key, char *name, TAG_tag_t *tags, uint64_t count)
{

	assert(0);
}

MANFST_prototag_t *MANFST_init_prototag_bool(char *key,char *name, bool boolean)
{


	TAG_argptr_t argptr =	TAG_get_fn(TAG_BOOL, TAG_FN_BOOL_INIT);

	TAG_ptr_t ptr = argptr.BOOL->init(boolean);
	TAG_tag_t *tag = TAG_init(ptr, TAG_BOOL);
	assert(tag);
	MANFST_prototag_t *prototag = MANFST_init_prototag_empty(key, name);
	assert(prototag);
	prototag->tag = tag;
	return prototag;
}
void MANFST_free_prototag(MANFST_prototag_t *prototag)
{
	free(prototag->buffer);
	free(prototag);

}

void MANFST_print_prototag(MANFST_prototag_t *prototag)
{


	char buffer1[prototag->key_size + 1];

	MANFST_get_prototag_key(prototag, buffer1, prototag->key_size + 1);

	printf("prototag: %s\n", buffer1);
	TAG_print(prototag->tag);
}


char *combine_strings_alloc(char *str1, uint64_t len1, char *str2, uint64_t len2)
{
	uint64_t size = len1 + len2;
	assert(len1 > 0);

	char *together = calloc(size, sizeof(char));
	assert(together);
	uint64_t c = 0;
	for(uint64_t i = 0; i < len1; ++i, ++c)
	{
		assert(str1[i] != 0);

		together[c] = str1[i];
	}
	if(len2 == 0)
	{
		return together;
	}
	for(uint64_t k = 0; k < len2; ++k, ++c)
	{
		assert(str2[k] != 0);


		together[c] = str2[k];
	}

	return together;
}