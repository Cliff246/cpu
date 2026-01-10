#ifndef __WS_OBJ_GLOBAL_HEADER__
#define __WS_OBJ_GLOBAL_HEADER__


#define OBJ_GLOBAL_LIST(X)\
	X(ERROR, int)		  \
	X(STAGE, int)		  \

#define OBJ_GLOBAL_STRUCT(X, Y) Y X;


typedef struct WS_OBJ_global
{
	OBJ_GLOBAL_LIST(OBJ_GLOBAL_STRUCT)
}OBJ_global_t;


#endif