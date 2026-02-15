#include "TAG_map.h"

#include "TAG_tag.h"
#include <assert.h>





TAG_map_arg_t init_map =
{
	.init = TAG_init_map
};

static TAG_ptr_t TAG_init_map(void)
{
	assert(0);
	TAG_ptr_t ptr;
	return ptr;
}

void TAG_map_free(TAG_ptr_t ptr)
{

}
void TAG_map_print(TAG_ptr_t ptr)
{

}

TAG_ptr_t TAG_map_copy(TAG_ptr_t ptr)
{
	assert(0);
	TAG_ptr_t copy;
	return copy;
}



TAG_prototype_vtable_t TAG_map_vtable =
{
	.free = TAG_map_free,
	.print = TAG_map_print,
	.copy = TAG_map_copy,
	.size = 1,
	.fn =
	{
		[TAG_FN_MAP_INIT].MAP = &init_map
	}
};