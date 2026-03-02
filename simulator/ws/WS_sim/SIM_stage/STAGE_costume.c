#include "STAGE_costume.h"
#include "TAG_tag.h"
#include "hashmap.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void STAGE_free_dataptr_costume(void *ptr)
{
	TAG_tag_t *tag = (TAG_tag_t *)ptr;
	TAG_free(tag);
}


STAGE_costume_t *STAGE_init_costume(char *key)
{
	STAGE_costume_t *costume = calloc(1, sizeof(STAGE_costume_t));
	costume->table = new_hash_table(20,STAGE_free_dataptr_costume);
	costume->key = strdup(key);
	return costume;
}


void STAGE_append_costume(STAGE_costume_t *costume, char *key, TAG_tag_t *tag)
{
	assert(0);
	addto_hash_table(costume->table, key, tag);
}