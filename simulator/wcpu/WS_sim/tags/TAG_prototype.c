#include "TAG_prototype.h"


#include "TAG_string.h"
#include "TAG_int.h"
#include "TAG_bool.h"


#define TAG_VTABLE_FILL(X, Y) [Y] = &X##_vtable ,

TAG_prototype_vtable_t *TAG_vtable_list[WS_SIM_TAG_LIST_COUNT] =
{
	WS_SIM_TAG_LIST(TAG_VTABLE_FILL)
};