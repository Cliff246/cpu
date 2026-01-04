#ifndef __WS_SIM_HND_PTR_HEADER__
#define __WS_SIM_HND_PTR_HEADER__

#include "HND_monolith.h"
#include "HND_bus.h"
#include "HND_service.h"


#define WS_SIM_HND_PTR_LIST(X)	\
	X(MONOLITH, monolith)		\
	X(SERVICE, service)			\
	X(BUS, bus)					\

#define WS_SIM_HND_PTR_ENUM_NAME(X) HND_TYPE_ ## X
#define WS_SIM_HND_PTR_ENUM(X, Y) WS_SIM_HND_PTR_ENUM_NAME(X),

typedef enum
{
	WS_SIM_HND_PTR_LIST(WS_SIM_HND_PTR_ENUM)
}HND_type_t;

#define WS_SIM_HND_PTR_STRUCT_NAME(X) WS_SIM_HND_ ## X
#define WS_SIM_HND_PTR_STRUCT(X, Y) typedef struct WS_SIM_HND_PTR_STRUCT_NAME(Y) HND_ ## Y ## _t ;
WS_SIM_HND_PTR_LIST(WS_SIM_HND_PTR_STRUCT);

#define WS_SIM_HND_PTR_UNION(X, Y) HND_ ## Y ## _t *X ;

typedef union
{
	WS_SIM_HND_PTR_LIST(WS_SIM_HND_PTR_UNION)
}HND_ptr_t;

#endif