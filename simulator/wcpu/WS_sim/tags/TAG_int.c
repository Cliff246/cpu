#include "TAG_int.h"

void TAG_int_free(TAG_ptr_t ptr)
{

}

TAG_prototype_vtable_t TAG_int_vtable =
{
	.free = TAG_int_free,
};
