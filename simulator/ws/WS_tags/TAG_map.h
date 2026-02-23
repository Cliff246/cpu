#ifndef __TAG_MAP_HEADER__
#define __TAG_MAP_HEADER__

#include "TAG_tag.h"
#include <stdint.h>
#include <sys/types.h>

enum WS_SIM_TAG_MAP_fn_type
{
	WS_SIM_TAG_FN_TYPE(MAP, INIT)
	WS_SIM_TAG_FN_TYPE(MAP, SET_KEY_STRING)
	WS_SIM_TAG_FN_TYPE(MAP, GET_KEY_STRING)

	WS_SIM_TAG_FN_TYPE(MAP, SET_KEY_INT)
	WS_SIM_TAG_FN_TYPE(MAP, GET_KEY_INT)


};

#define TAG_MAPKEY_LIST(X)\
	X(INT, int64_t		) \
	X(STR, char *		) \


#define TAG_MAPKEY_TYPE_NAME(X) TAG_MAPKEY_ ## X
#define TAG_MAPKEY_TYPE_ENUM(X, Y) TAG_MAPKEY_TYPE_NAME(X),

#define TAG_MAPKEY_TYPE_COUNT (0 TAG_MAPKEY_LIST(COUNTER2D))

enum TAG_mapkey_type
{

	TAG_MAPKEY_LIST(TAG_MAPKEY_TYPE_ENUM)
};

#define TAG_MAPKEY_UNION(X, Y) Y X;

union TAG_mapkey
{
	TAG_MAPKEY_LIST(TAG_MAPKEY_UNION)
};

#define TAG_MAPKEY_HASH_NAME(X) TAG_hash_ ## X ## _mapkey
#define TAG_MAPKEY_FREE_NAME(X) TAG_free_ ## X ## _mapkey
#define TAG_MAPKEY_INIT_NAME(X) TAG_init_ ## X ## _mapkey
#define TAG_MAPKEY_CMP_NAME(X) TAG_cmp_ ## X ## _mapkey

#define TAG_MAPKEY_FREE(X, Y) void TAG_MAPKEY_FREE_NAME(X)(union TAG_mapkey key);
#define TAG_MAPKEY_HASH(X, Y) int64_t TAG_MAPKEY_HASH_NAME(X)(union TAG_mapkey key);
#define TAG_MAPKEY_INIT(X, Y) union TAG_mapkey TAG_MAPKEY_INIT_NAME(X)(union TAG_mapkey key);
#define TAG_MAPKEY_CMP(X, Y) bool TAG_MAPKEY_CMP_NAME(X)(union TAG_mapkey key1, union TAG_mapkey key2);


TAG_MAPKEY_LIST(TAG_MAPKEY_FREE)
TAG_MAPKEY_LIST(TAG_MAPKEY_HASH)
TAG_MAPKEY_LIST(TAG_MAPKEY_INIT)
TAG_MAPKEY_LIST(TAG_MAPKEY_CMP)

struct mapkey_vtable
{
	enum TAG_mapkey_type type;
	void (*free)(union TAG_mapkey key);
	int64_t (*hash)(union TAG_mapkey key);
	union TAG_mapkey (*init)(union TAG_mapkey key);
	bool (*cmp)(union TAG_mapkey key1, union TAG_mapkey key2);
};





struct TAG_mapelm
{
	TAG_tag_t *tag;
	union TAG_mapkey key;
	enum TAG_mapkey_type type;
	int64_t hash;
};

struct WS_SIM_TAG_map
{

	uint64_t allocated;
	uint64_t count;
	struct TAG_mapelm *map;
};


//key functions
void TAG_mapkey_free(union TAG_mapkey key, enum TAG_mapkey_type type);
int64_t TAG_mapkey_hash(union TAG_mapkey key, enum TAG_mapkey_type type);
union TAG_mapkey TAG_mapkey_init(union TAG_mapkey key, enum TAG_mapkey_type type);
bool TAG_mapkey_cmp(union TAG_mapkey key1, union TAG_mapkey key2, enum TAG_mapkey_type type);

//helpers
void TAG_map_realloc(TAG_ptr_t ptr);


//generic map functions required by tag
void TAG_map_free(TAG_ptr_t ptr);
void TAG_map_print(TAG_ptr_t ptr);
TAG_ptr_t TAG_map_copy(TAG_ptr_t ptr);




static TAG_ptr_t TAG_init_map(void);

static bool TAG_set_key_string_map(TAG_ptr_t ptr, char *key, TAG_tag_t *tag);
static TAG_tag_t *TAG_get_key_string_map(TAG_ptr_t ptr, char *key);

static bool TAG_set_key_int_map(TAG_ptr_t ptr, int64_t key, TAG_tag_t *tag);
static TAG_tag_t *TAG_get_key_int_map(TAG_ptr_t ptr, int64_t key);





extern TAG_prototype_vtable_t TAG_map_vtable;

union WS_SIM_TAG_map_arg
{
	TAG_ptr_t (*init)(void);

	bool (*set_key_string)(TAG_ptr_t ptr, char *key, TAG_tag_t *tag);
	TAG_tag_t *(*get_key_string)(TAG_ptr_t ptr, char *key);

	bool (*set_key_int)(TAG_ptr_t ptr, int64_t key, TAG_tag_t *tag);
	TAG_tag_t *(*get_key_int)(TAG_ptr_t ptr, int64_t key);

};

#endif