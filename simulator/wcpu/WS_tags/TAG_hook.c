#include "TAG_hook.h"

#include "TAG_map.h"

#include "TAG_tag.h"
#include <assert.h>





TAG_hook_arg_t init_hook =
{
	.init = TAG_init_hook
};

static TAG_ptr_t TAG_init_hook(void)
{
	assert(0);
	TAG_ptr_t ptr;
	return ptr;
}

void TAG_hook_free(TAG_ptr_t ptr)
{

}
void TAG_hook_print(TAG_ptr_t ptr)
{

}

TAG_ptr_t TAG_hook_copy(TAG_ptr_t ptr)
{
	assert(0);
	TAG_ptr_t copy;
	return copy;
}



TAG_prototype_vtable_t TAG_hook_vtable =
{
	.free = TAG_hook_free,
	.print = TAG_hook_print,
	.copy = TAG_hook_copy,
	.size = 1,
	.fn =
	{
		[TAG_FN_HOOK_INIT].HOOK = &init_hook
	}
};