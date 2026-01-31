#ifndef __WS_SIM_TAG_PROTOTYPE_HEADER__
#define __WS_SIM_TAG_PROTOTYPE_HEADER__

#include "commons.h"

#define WS_SIM_TAG_LIST(X)\
	X(TAG_string, STRING) \
	X(TAG_int, INT)		  \
	X(TAG_bool, BOOL)		\

#define WS_SIM_TAG_ENUM(X, Y) Y,

#define WS_SIM_TAG_LIST_COUNT (0 WS_SIM_TAG_LIST(COUNTER2D))

typedef enum WS_SIM_TAG_tagtype
{
	WS_SIM_TAG_LIST(WS_SIM_TAG_ENUM)
}TAG_type_t;

#define WS_SIM_TAG_STRUCT_NAME(X) WS_SIM_ ## X
#define WS_SIM_TAG_STRUCT(X, Y) typedef struct WS_SIM_TAG_STRUCT_NAME(X) X##_t;

WS_SIM_TAG_LIST(WS_SIM_TAG_STRUCT)

#define WS_SIM_TAG_UNION(X, Y) X##_t *Y;

typedef union WS_SIM_TAG_ptr
{
	WS_SIM_TAG_LIST(WS_SIM_TAG_UNION)
}TAG_ptr_t;

typedef struct WS_SIM_TAG_prototype_vtable
{
	void (*free)(TAG_ptr_t ptr);
}TAG_prototype_vtable_t;

extern TAG_prototype_vtable_t *TAG_vtable_list[WS_SIM_TAG_LIST_COUNT];


typedef struct WS_SIM_TAG_prototype
{
	TAG_type_t type;
	TAG_ptr_t ptr;
}TAG_prototype_t;

#endif