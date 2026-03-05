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


	WS_SIM_TAG_FN_TYPE(MAP, ITER_RESET)
	WS_SIM_TAG_FN_TYPE(MAP, ITER_UP)
	WS_SIM_TAG_FN_TYPE(MAP, ITER_DOWN)
	WS_SIM_TAG_FN_TYPE(MAP, ITER_END)
	WS_SIM_TAG_FN_TYPE(MAP, ITER_IS_KEY_STR)
	WS_SIM_TAG_FN_TYPE(MAP, ITER_IS_KEY_INT)
	WS_SIM_TAG_FN_TYPE(MAP, ITER_GET_KEY_STR)
	WS_SIM_TAG_FN_TYPE(MAP, ITER_GET_KEY_INT)
	WS_SIM_TAG_FN_TYPE(MAP, ITER_GET_VALUE)
	WS_SIM_TAG_FN_TYPE(MAP, GET_SIZE)

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
	uint64_t iter;
	uint64_t scroll_iter;
	struct TAG_mapelm *map;
};


//key functions
void TAG_mapkey_free(union TAG_mapkey key, enum TAG_mapkey_type type);
int64_t TAG_mapkey_hash(union TAG_mapkey key, enum TAG_mapkey_type type);
union TAG_mapkey TAG_mapkey_init(union TAG_mapkey key, enum TAG_mapkey_type type);
bool TAG_mapkey_cmp(union TAG_mapkey key1, union TAG_mapkey key2, enum TAG_mapkey_type type);

//helpers
void TAG_map_realloc(TAG_ptr_t ptr);

void TAG_map_print_entries(TAG_ptr_t ptr);

struct TAG_mapelm *TAG_map_get_element(TAG_tag_t *tag, uint64_t i);

//generic map functions required by tag
void TAG_map_free(TAG_ptr_t ptr);
void TAG_map_print(TAG_ptr_t ptr, uint64_t tab);

TAG_ptr_t TAG_map_copy(TAG_ptr_t ptr);




static TAG_ptr_t TAG_init_map(void);

static bool TAG_set_key_string_map(TAG_tag_t *ptr, char *key, TAG_tag_t *tag);
static TAG_tag_t *TAG_get_key_string_map(TAG_tag_t *ptr, char *key);

static bool TAG_set_key_int_map(TAG_tag_t *ptr, int64_t key, TAG_tag_t *tag);
static TAG_tag_t *TAG_get_key_int_map(TAG_tag_t *ptr, int64_t key);


static bool TAG_iter_reset_map(TAG_tag_t *tag);
static bool TAG_iter_up_map(TAG_tag_t *tag);
static bool TAG_iter_down_map(TAG_tag_t *tag);
static bool TAG_iter_is_end_map(TAG_tag_t *tag);
static bool TAG_iter_is_key_str_map(TAG_tag_t *tag);
static bool TAG_iter_is_key_int_map(TAG_tag_t *tag);
static char *TAG_iter_get_key_str_map(TAG_tag_t *tag);
static int64_t TAG_iter_get_key_int_map(TAG_tag_t *tag);
static TAG_tag_t *TAG_iter_get_value(TAG_tag_t *tag);
static uint64_t TAG_get_size(TAG_tag_t *tag);




extern TAG_prototype_vtable_t TAG_map_vtable;

union WS_SIM_TAG_map_arg
{
	TAG_ptr_t (*init)(void);

	bool (*set_key_string)(TAG_tag_t *ptr, char *key, TAG_tag_t *tag);
	TAG_tag_t *(*get_key_string)(TAG_tag_t *ptr, char *key);

	bool (*set_key_int)(TAG_tag_t *ptr, int64_t key, TAG_tag_t *tag);
	TAG_tag_t *(*get_key_int)(TAG_tag_t *ptr, int64_t key);


	bool (*iter_reset)(TAG_tag_t *tag);
	bool (*iter_up)(TAG_tag_t *tag);
	bool (*iter_down)(TAG_tag_t *tag);
	bool (*iter_is_end)(TAG_tag_t *tag);
	bool (*iter_is_key_str)(TAG_tag_t *tag);
	bool (*iter_is_key_int)(TAG_tag_t *tag);
	char *(*iter_get_key_str)(TAG_tag_t *tag);
	int64_t (*iter_get_key_int)(TAG_tag_t *tag);
	TAG_tag_t *(*iter_get_value)(TAG_tag_t *tag);
	uint64_t (*get_size)(TAG_tag_t *tag);

};

#endif
