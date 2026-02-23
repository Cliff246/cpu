#include "CFG_commons.h"
#include "TAG_tag.h"


void CFG_free_tag_hashtable(void *v)
{
	TAG_tag_t *tag = (TAG_tag_t *)v;

	TAG_free(tag);
}
