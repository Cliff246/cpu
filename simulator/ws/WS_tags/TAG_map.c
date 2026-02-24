#include "TAG_map.h"

#include "TAG_tag.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAG_MAPKEY_VTABLE_LIST_FILL(X, Y)		\
	[TAG_MAPKEY_TYPE_NAME(X)] =					\
	{											\
		.type = TAG_MAPKEY_TYPE_NAME(X),		\
		.free = TAG_MAPKEY_FREE_NAME(X),		\
		.hash = TAG_MAPKEY_HASH_NAME(X),		\
		.init = TAG_MAPKEY_INIT_NAME(X),		\
		.cmp = TAG_MAPKEY_CMP_NAME(X),			\
	},											\

struct mapkey_vtable mapkey_vtable_list[TAG_MAPKEY_TYPE_COUNT] =
{
	TAG_MAPKEY_LIST(TAG_MAPKEY_VTABLE_LIST_FILL)
};

TAG_map_arg_t init_map =
{
	.init = TAG_init_map
};

TAG_map_arg_t set_key_string_map =
{
	.set_key_string = TAG_set_key_string_map
};

TAG_map_arg_t set_key_int_map =
{
	.set_key_int = TAG_set_key_int_map
};

TAG_map_arg_t get_key_string_map =
{
	.get_key_string = TAG_get_key_string_map
};

TAG_map_arg_t get_key_int_map =
{
	.get_key_int =  TAG_get_key_int_map
};


//mapkey types functions
//mapkey hash

int64_t TAG_hash_INT_mapkey(union TAG_mapkey key)
{
	return key.INT;
}

int64_t TAG_hash_STR_mapkey(union TAG_mapkey key)
{
	int64_t result = 0x0123456789abcdef;
	char *keystr = key.STR;
	while (*keystr)
	{
 		result = (result << 5) - result + *keystr++;
	}
	return result;
}

//mapkey frees


void TAG_free_INT_mapkey(union TAG_mapkey key)
{
	return;
}

void TAG_free_STR_mapkey(union TAG_mapkey key)
{
	free(key.STR);
	return;
}

//mapkey init


union TAG_mapkey TAG_init_INT_mapkey(union TAG_mapkey key)
{
	union TAG_mapkey mapkey;
	mapkey.INT = key.INT;
	return mapkey;
}

union TAG_mapkey TAG_init_STR_mapkey(union TAG_mapkey key)
{
	union TAG_mapkey mapkey;
	mapkey.STR = strdup(key.STR);
	return mapkey;
}

//mapkey cmp


bool TAG_cmp_INT_mapkey(union TAG_mapkey key1, union TAG_mapkey key2)
{
	return (key1.INT == key2.INT)? true : false;
}

bool TAG_cmp_STR_mapkey(union TAG_mapkey key1, union TAG_mapkey key2)
{
	int cmp = strcmp(key1.STR, key2.STR);
	return (!cmp)? true : false;
}


//generic functions


void TAG_mapkey_free(union TAG_mapkey key, enum TAG_mapkey_type type)
{
	assert(mapkey_vtable_list[type].free != NULL);
	mapkey_vtable_list[type].free(key);
}

int64_t TAG_mapkey_hash(union TAG_mapkey key, enum TAG_mapkey_type type)
{
	assert(mapkey_vtable_list[type].hash != NULL);

	int64_t hash = mapkey_vtable_list[type].hash(key);
	hash &= 0x7fffffffffffffff;
	return hash;
}

union TAG_mapkey TAG_mapkey_init(union TAG_mapkey key, enum TAG_mapkey_type type)
{
	assert(mapkey_vtable_list[type].init != NULL);

	return mapkey_vtable_list[type].init(key);

}



bool TAG_mapkey_cmp(union TAG_mapkey key1, union TAG_mapkey key2, enum TAG_mapkey_type type)
{
	assert(mapkey_vtable_list[type].cmp != NULL);
	return mapkey_vtable_list[type].cmp(key1, key2);

}

//helper functions
void TAG_map_realloc(TAG_ptr_t ptr)
{
	TAG_map_t *map = ptr.MAP;
	const uint64_t old_max = map->allocated;
	uint64_t new_max = (map->allocated * 2) + 1;

	struct TAG_mapelm *new_elms = calloc(new_max, sizeof(struct TAG_mapelm));


	for(uint64_t i = 0; i < new_max; ++i)
	{
		struct TAG_mapelm *elm = &new_elms[i];
		elm->hash = -1;
	}

	for(uint64_t k = 0; k < old_max; ++k)
	{
		struct TAG_mapelm *elm = &map->map[k];

		if(elm->hash < 0)
		{
			continue;
		}

		uint64_t pos = elm->hash % new_max;
		bool found = false;
		for(uint64_t j = 0; j < new_max; ++j)
		{
			struct TAG_mapelm *tmp = &new_elms[pos];
			if(tmp->hash < 0)
			{
				tmp->key = elm->key;
				tmp->tag = elm->tag;
				tmp->type = elm->type;
				tmp->hash = elm->hash;
				found = true;
				break;
			}

			pos = (pos + 1) % new_max;
		}
		assert(found == true && "how did you manage to do this");


	}

	free(map->map);
	map->map = new_elms;
	map->allocated = new_max;

}

//default functions

void TAG_map_free(TAG_ptr_t ptr)
{
	assert(0);
}
void TAG_map_print(TAG_ptr_t ptr)
{
	TAG_map_t *map = ptr.MAP;
	printf("map %ld\n", map->count);
	for(uint64_t i = 0; i < map->allocated; ++i)
	{
		struct TAG_mapelm *elm = &map->map[i];
		if(elm->hash != -1)
		{

			if(elm->type == TAG_MAPKEY_STR)
			{
				TAG_print_index_str(elm->tag, elm->key.STR);

			}
			else
			{
				TAG_print_index_int(elm->tag,elm->key.INT);

			}
		}
	}
}

TAG_ptr_t TAG_map_copy(TAG_ptr_t ptr)
{
	assert(0);
	TAG_ptr_t copy;
	return copy;
}




//tag functions



static TAG_ptr_t TAG_init_map(void)
{
	TAG_map_t *map = calloc(1, sizeof(TAG_map_t));
	assert(map);
	map->allocated = 10;
	map->map = calloc(map->allocated, sizeof(struct TAG_mapelm));

	for(uint64_t i = 0; i < map->allocated; ++i)
	{
		struct TAG_mapelm *elm = &map->map[i];
		elm->hash = -1;
	}

	map->count = 0;

	TAG_ptr_t ptr;
	ptr.MAP = map;
	return ptr;
}

bool TAG_set_key_map(TAG_ptr_t ptr, union TAG_mapkey key, enum TAG_mapkey_type type, TAG_tag_t *tag)
{


	//TODO fix this
	TAG_map_t *map = ptr.MAP;
	if(map->allocated <= map->count)
	{
		TAG_map_realloc(ptr);
	}

	int64_t hash = TAG_mapkey_hash(key, type);
	uint64_t start = hash % map->allocated;

	for(uint64_t i = 0; i < map->allocated; ++i)
	{
		struct TAG_mapelm *mapelm = &map->map[start];


		int64_t tmp_hash = mapelm->hash;
		if(tmp_hash == -1)
		{
			mapelm->hash = hash;
			mapelm->key = TAG_mapkey_init(key, type);
			mapelm->tag = tag;
			mapelm->type = type;
			map->count++;
			//TAG_print(mapelm->tag);
			return true;
		}
		else if(tmp_hash == hash)
		{
			if(mapelm->type == type)
			{
				bool same = TAG_mapkey_cmp(key, mapelm->key, type);
				if(same == true)
				{
					//should reset
					assert(0 && "mapkey is the same");
				}
			}
			start = (start + 1) % map->allocated;
		}
		else
		{
			start = (start + 1) % map->allocated;
		}
	}
	return false;
}

TAG_tag_t *TAG_get_key_map(TAG_ptr_t ptr, union TAG_mapkey key, enum TAG_mapkey_type type)
{
	TAG_map_t *map = ptr.MAP;
	int64_t hash = TAG_mapkey_hash(key, type);
	uint64_t start = hash % map->allocated;

	for(uint64_t i = 0; i < map->allocated; ++i)
	{
		struct TAG_mapelm *mapelm = &map->map[start];


		int64_t tmp_hash = mapelm->hash;


		if(tmp_hash == hash)
		{
			if(mapelm->type == type)
			{
				bool same = TAG_mapkey_cmp(key, mapelm->key, type);
				if(same == true)
				{
					return mapelm->tag;
				}
			}
			start = (start + 1) %  map->allocated;
		}
		else if(tmp_hash == -1)
		{
			return NULL;
		}
		else
		{
			start = (start + 1) %  map->allocated;
		}
	}

	return NULL;

}



static bool TAG_set_key_string_map(TAG_tag_t *ptr, char *key, TAG_tag_t *tag)
{
	union TAG_mapkey mapkey;
	mapkey.STR = key;
	//printf("mapkey: <%s>\n", mapkey);
	return TAG_set_key_map(ptr->ptr, mapkey, TAG_MAPKEY_STR, tag);
}

static TAG_tag_t *TAG_get_key_string_map(TAG_tag_t *ptr, char *key)
{
	union TAG_mapkey mapkey;
	mapkey.STR = key;
	return TAG_get_key_map(ptr->ptr, mapkey, TAG_MAPKEY_STR);
}

static bool TAG_set_key_int_map(TAG_tag_t *ptr, int64_t key, TAG_tag_t *tag)
{
	union TAG_mapkey mapkey;
	mapkey.INT = key;
	return TAG_set_key_map(ptr->ptr, mapkey, TAG_MAPKEY_INT, tag);
}

static TAG_tag_t *TAG_get_key_int_map(TAG_tag_t *ptr, int64_t key)
{
	union TAG_mapkey mapkey;

	mapkey.INT = key;

	return TAG_get_key_map(ptr->ptr, mapkey, TAG_MAPKEY_INT);

}









TAG_prototype_vtable_t TAG_map_vtable =
{
	.free = TAG_map_free,
	.print = TAG_map_print,
	.copy = TAG_map_copy,
	.size = 5,
	.fn =
	{
		[TAG_FN_MAP_INIT].MAP = &init_map,
		[TAG_FN_MAP_SET_KEY_STRING].MAP = &set_key_string_map,
		[TAG_FN_MAP_GET_KEY_STRING].MAP = &get_key_string_map,
		[TAG_FN_MAP_SET_KEY_INT].MAP = &set_key_int_map,
		[TAG_FN_MAP_GET_KEY_INT].MAP = &get_key_int_map,
	}
};