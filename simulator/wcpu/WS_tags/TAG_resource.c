#include "TAG_resource.h"
#include "TAG_tag.h"
#include <assert.h>


TAG_resource_arg_t init_resource;



TAG_resource_arg_t init_resource =
{
	.init = TAG_init_resource
};

static TAG_ptr_t TAG_init_resource(void)
{
	assert(0);
	TAG_ptr_t ptr;
	return ptr;
}

void TAG_resource_free(TAG_ptr_t ptr)
{

}
void TAG_resource_print(TAG_ptr_t ptr)
{

}

TAG_ptr_t TAG_resource_copy(TAG_ptr_t ptr)
{
	assert(0);
	TAG_ptr_t copy;
	return copy;
}



TAG_prototype_vtable_t TAG_resource_vtable =
{
	.free = TAG_resource_free,
	.print = TAG_resource_print,
	.copy = TAG_resource_copy,
	.size = 1,
	.fn =
	{
		[TAG_FN_RESOURCE_INIT].RESOURCE = &init_resource
	}
};