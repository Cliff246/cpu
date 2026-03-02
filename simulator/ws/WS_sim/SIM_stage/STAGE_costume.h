#ifndef __STAGE_COSTUME_HEADER__
#define __STAGE_COSTUME_HEADER__


#include "TAG_tag.h"
#include "hashmap.h"


typedef struct STAGE_costume
{
	char *key;
	p_hashtable_t table;
}STAGE_costume_t;



STAGE_costume_t *STAGE_init_costume(char *key);

void STAGE_append_costume(STAGE_costume_t *costume, char *key, TAG_tag_t *tag);

#endif