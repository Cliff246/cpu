#ifndef __WS_MOD_BINDING_HEADER__
#define __WS_MOD_BINDING_HEADER__

#include "MOD_bindser.h"
#include "MOD_bindobj.h"

#define MOD_BIND_TYPE_LIST(X)\
	X(OBJECT	,obj)				 \
	X(SERVICE	,ser)				 \


#define MOD_BIND_TYPE_NAME(X) MOD_BINDING_ ## X
#define MOD_BIND_TYPE_ENUM(X,Y) MOD_BIND_TYPE_NAME(X),

typedef enum WS_MOD_binding_type
{
	MOD_BIND_TYPE_LIST(MOD_BIND_TYPE_ENUM)
}MOD_binding_type_t;

#define MOD_BIND_UNION(X, Y) MOD_bind ## Y ## _t *X;

typedef union WS_MOD_bind
{
	MOD_BIND_TYPE_LIST(MOD_BIND_UNION)
}MOD_bind_t;

typedef struct WS_MOD_binding
{
	int version;
	MOD_binding_type_t type;
	MOD_bind_t binding;
}MOD_binding_t;

#endif