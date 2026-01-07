#ifndef __WS_OBJECT_VALUE_HEADER__
#define __WS_OBJECT_VALUE_HEADER__

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdalign.h>


#define WS_OBJ_VALUE_LIST(X) 			\
	X(BOOL		, bool		, 1)		  	\
	X(U64		, uint64_t	, 1)	 		\
	X(I64		, int64_t	, 1)			\
	X(STRING	, char		, 8)			\


#define WS_OBJ_VALUE_ENUM_TYPE(X) WS_OBJ_TYPE_ ## X
#define WS_OBJ_VALUE_ENUM(X, Y, Z) WS_OBJ_VALUE_ENUM_TYPE(X),


#define WS_OBJ_COUNT (0 WS_OBJ_LIST(COUNTER2D))

typedef enum WS_OBJ_Type
{
	WS_OBJ_VALUE_ENUM_TYPE(UNKNOWN),
	WS_OBJ_VALUE_LIST(WS_OBJ_VALUE_ENUM)

}OBJ_type_t;

#define WS_OBJ_VALUE_UNION(X, Y, Z) Y X [Z];

typedef union WS_OBJ_value_union
{
	WS_OBJ_VALUE_LIST(WS_OBJ_VALUE_UNION)
}OBJ_value_union_t;

static_assert(8 == sizeof(OBJ_value_union_t), "that should always be aligned" );

typedef struct WS_OBJ_value_primitive
{
	_Alignas(8) OBJ_value_union_t content;
}OBJ_primitive_t;

typedef struct WS_OBJ_value
{
	int refcount;
	OBJ_type_t type;
	OBJ_primitive_t *ary;
	//supposed to ensure
	uint64_t size;
	uint64_t alloc_bytes;
}OBJ_value_t;


#endif