#include "SIM_commons.h"
#include "TAG_tag.h"

void SIM_free_tag_table_elem(void *v)
{
	TAG_tag_t *tag = (TAG_tag_t *)v;

	TAG_free(tag);
}
