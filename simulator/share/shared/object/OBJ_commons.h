#ifndef __WS_OBJ_COMMONS_HEADER__
#define __WS_OBJ_COMMONS_HEADER__

#include <stdint.h>
#include <stdbool.h>
#include "OBJ_handle.h"
//cold is allowed on cold path
//hot is allowed on hot path


typedef struct WS_OBJ_object OBJ_object_t;
#define OBJ_COMMON_UTILS_LIST(X)\
	X(FUNC	,COLD	,ALLOC			,uint64_t	,uint64_t id)		  \
	X(FLAG	,HOT	,ERROR			,void		,const char *)		  \
	X(FUNC	,HOT	,LOG			,void		,const char *)		  \
	X(FLAG	,COLD	,GET_TAG		,char *		,const char *)		  \
	X(FLAG	,COLD	,SET_TAG		,void 		,const char *)		  \
	X(FUNC	,COLD	,FREE			,void		,uint64_t	 )		  \
	X(FLAG	,HOT	,SET_STOP		,void		,uint64_t	 )		  \
	X(FLAG	,COLD	,GET_HND		,OBJ_hid_t	,int	 )		  \


#define OBJ_COMMON_UTILS_NAME(A, B) OBJ_UTIL_##A##_##B
#define OBJ_COMMON_UTILS_ENUM(A, B, C, D, ...) OBJ_COMMON_UTILS_NAME(A,C),
typedef enum WS_OBJ_util_type
{
	OBJ_COMMON_UTILS_LIST(OBJ_COMMON_UTILS_ENUM)
}OBJ_util_type_t;

#define OBJ_COMMON_UTILS_FNPTR(A, B, C, D, ...) typedef D (*UTIL_ ## C ## _t)(OBJ_object_t *obj, OBJ_hnd_t *hnd, __VA_ARGS__);
OBJ_COMMON_UTILS_LIST(OBJ_COMMON_UTILS_FNPTR)
#define OBJ_COMMON_UTILS_STRUCT(A, B, C, D, ...) UTIL_ ## C ## _t C;

typedef struct WS_OBJ_utils
{
	OBJ_COMMON_UTILS_LIST(OBJ_COMMON_UTILS_STRUCT)
}OBJ_utils_t;

typedef struct WS_OBJ_commons
{
	OBJ_utils_t utils;
}OBJ_commons_t;



#endif