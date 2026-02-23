#ifndef __WS_SIM_TAG_PROTOTYPE_HEADER__
#define __WS_SIM_TAG_PROTOTYPE_HEADER__

#include "commons.h"
#include <stdint.h>

#define WS_SIM_TAG_LIST(X)\
	X(TAG_string, STRING,	S) \
	X(TAG_int, INT, 		I)		  \
	X(TAG_bool, BOOL, 		B)		\
	X(TAG_list, LIST, 		L)		\
	X(TAG_map, 	MAP, 		M)		\

#define WS_SIM_TAG_ENUM_NAME(X) TAG_ ## X
#define WS_SIM_TAG_ENUM(X, Y, Z) WS_SIM_TAG_ENUM_NAME(Y),

#define WS_SIM_TAG_LIST_COUNT (0 WS_SIM_TAG_LIST(COUNTER3D))

typedef enum WS_SIM_TAG_tagtype
{
	WS_SIM_TAG_LIST(WS_SIM_TAG_ENUM)
}TAG_type_t;

#define WS_SIM_TAG_STRUCT_NAME(X) WS_SIM_ ## X
#define WS_SIM_TAG_STRUCT(X, Y, Z) typedef struct WS_SIM_TAG_STRUCT_NAME(X) X##_t;

#define WS_SIM_TAG_ARGFN_NAME(X) WS_SIM_ ## X ## _arg
#define WS_SIM_TAG_ARGFN(X, Y, Z) typedef union WS_SIM_TAG_ARGFN_NAME(X) X##_arg_t;

#define WS_SIM_TAG_ARGFN_TYPE_NAME(X) WS_SIM_ ## X ## _fn_type
#define WS_SIM_TAG_ARGFN_TYPE_ENUM(X, Y, Z) typedef enum WS_SIM_TAG_ARGFN_TYPE_NAME(X) X##_fn_type_t;

WS_SIM_TAG_LIST(WS_SIM_TAG_ARGFN_TYPE_ENUM)
WS_SIM_TAG_LIST(WS_SIM_TAG_ARGFN)
WS_SIM_TAG_LIST(WS_SIM_TAG_STRUCT)

#define WS_SIM_TAG_UNION(X, Y, Z) X##_t *Y;
#define WS_SIM_TAG_FN_TYPE(X, Y) TAG_FN_ ## X ## _ ## Y ,

typedef union WS_SIM_TAG_ptr
{
	void *SELF;
	WS_SIM_TAG_LIST(WS_SIM_TAG_UNION)
}TAG_ptr_t;

#define WS_SIM_TAG_ARGPTR(X, Y, Z) X##_arg_t *Y;

typedef union WS_SIM_TAG_argptr
{
	WS_SIM_TAG_LIST(WS_SIM_TAG_ARGPTR)
}TAG_argptr_t;





typedef struct WS_SIM_TAG_prototype_vtable
{

	void (*free)(TAG_ptr_t ptr);
	void (*print)(TAG_ptr_t ptr);
	TAG_ptr_t (*copy)(TAG_ptr_t ptr);
	uint64_t size;
	TAG_argptr_t fn[];

}TAG_prototype_vtable_t;

extern TAG_prototype_vtable_t *TAG_vtable_list[WS_SIM_TAG_LIST_COUNT];
extern char *TAG_type_string_list[WS_SIM_TAG_LIST_COUNT];
typedef struct WS_SIM_TAG_tag
{
	TAG_type_t type;
	TAG_ptr_t ptr;
}TAG_tag_t;

void TAG_free(TAG_tag_t *tag);
void TAG_print(TAG_tag_t *tag);
TAG_tag_t *TAG_copy(TAG_tag_t *tag);



TAG_argptr_t TAG_get_fn(TAG_type_t type, uint64_t index);
TAG_tag_t *TAG_init(TAG_ptr_t ptr, TAG_type_t type);

#endif