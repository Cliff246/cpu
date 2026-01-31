#include "TAG_bool.h"

void TAG_bool_free(TAG_ptr_t ptr)
{

}

TAG_prototype_vtable_t TAG_bool_vtable =
{
	.free = TAG_bool_free,
};