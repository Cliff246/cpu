#ifndef __MAP__
#define __MAP__

//todo

#include <stdint.h>
#include <stdbool.h>

#define MAP_KEY_LIST(X)	\
	X(STR, char *)	   	\
	X(PTR, void *)	   	\
	X(I64, int64_t)  	\
	X(U64, uint64_t)	\
	X(F32, float)		\
	X(F64, double)		\

#define MAP_KEY_NAME(a,b) M ## a
#define MAP_KEY_ENUM(a,b) MAP_KEY_NAME(a,b),

typedef enum map_key_type
{
	MAP_KEY_LIST(MAP_KEY_ENUM)
}mapkey_type_t;

#define MAP_KEY_UNION(a,b) b a;

typedef union map_key_union
{
	MAP_KEY_LIST(MAP_KEY_UNION)
}mapkey_union_t;

typedef struct mapkey
{
	mapkey_type_t type;
	mapkey_union_t key;
}mapkey_t, *p_mapkey_t;

typedef struct map_value
{

}mapval_t, *p_mapval_t;

typedef struct map_element
{
	mapkey_t key;
}mapelm_t, *p_mapelm_t;


int64_t maphash();




typedef struct map_container
{

}map_t, *p_map_t;

#endif