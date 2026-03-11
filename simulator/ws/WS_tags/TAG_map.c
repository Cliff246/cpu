#include "TAG_map.h"

#include "TAG_tag.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

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

//key functions
void TAG_mapkey_free(union TAG_mapkey key, enum TAG_mapkey_type type);
int64_t TAG_mapkey_hash(union TAG_mapkey key, enum TAG_mapkey_type type);
union TAG_mapkey TAG_mapkey_init(union TAG_mapkey key, enum TAG_mapkey_type type);
bool TAG_mapkey_cmp(union TAG_mapkey key1, union TAG_mapkey key2, enum TAG_mapkey_type type);

//helpers
void TAG_map_realloc(TAG_ptr_t ptr);

void TAG_map_print_entries(TAG_ptr_t ptr);

struct TAG_mapelm *TAG_map_get_element(TAG_tag_t *tag, uint64_t i);

static TAG_ptr_t TAG_init_map(void);
static bool TAG_set_key_string_map(TAG_tag_t *ptr, char *key, TAG_tag_t *tag);
static TAG_tag_t *TAG_get_key_string_map(TAG_tag_t *ptr, char *key);
static bool TAG_remove_key_string_map(TAG_tag_t *ptr, char *key);
static bool TAG_set_key_int_map(TAG_tag_t *ptr, int64_t key, TAG_tag_t *tag);
static TAG_tag_t *TAG_get_key_int_map(TAG_tag_t *ptr, int64_t key);
static bool TAG_remove_key_int_map(TAG_tag_t *ptr, int64_t key);

static bool TAG_iter_start_map(TAG_tag_t *tag);
static bool TAG_iter_reset_map(TAG_tag_t *tag);
static bool TAG_iter_up_map(TAG_tag_t *tag);
static bool TAG_iter_down_map(TAG_tag_t *tag);
static bool TAG_iter_is_end_map(TAG_tag_t *tag);
static bool TAG_iter_end_map(TAG_tag_t *tag);

static bool TAG_iter_is_key_str_map(TAG_tag_t *tag);
static bool TAG_iter_is_key_int_map(TAG_tag_t *tag);
static char *TAG_iter_get_key_str_map(TAG_tag_t *tag);
static int64_t TAG_iter_get_key_int_map(TAG_tag_t *tag);
static TAG_tag_t *TAG_iter_get_value(TAG_tag_t *tag);
static uint64_t TAG_get_size(TAG_tag_t *tag);


TAG_map_arg_t init_map =
{
	.init = TAG_init_map
};

//str keys
TAG_map_arg_t set_key_string_map =
{
	.set_key_string = TAG_set_key_string_map
};


TAG_map_arg_t get_key_string_map =
{
	.get_key_string = TAG_get_key_string_map
};

TAG_map_arg_t rem_key_string_map =
{
	.rem_key_string = TAG_remove_key_string_map
};

//int keys
TAG_map_arg_t set_key_int_map =
{
	.set_key_int = TAG_set_key_int_map
};

TAG_map_arg_t get_key_int_map =
{
	.get_key_int =  TAG_get_key_int_map
};

TAG_map_arg_t rem_key_int_map =
{
	.rem_key_int= TAG_remove_key_int_map
};

//iter
//
//

TAG_map_arg_t iter_start_map =
{
	.iter_start = TAG_iter_start_map
};

TAG_map_arg_t iter_reset_map =
{
	.iter_reset = TAG_iter_reset_map
};

TAG_map_arg_t iter_up_map =
{
	.iter_up = TAG_iter_up_map,
};

TAG_map_arg_t iter_down_map =
{
	.iter_down = TAG_iter_down_map,
};
TAG_map_arg_t iter_is_end_map =
{
	.iter_is_end = TAG_iter_is_end_map
};

TAG_map_arg_t iter_end_map =
{
	.iter_end = TAG_iter_end_map
};

TAG_map_arg_t is_key_str_map =
{
	.iter_is_key_str = TAG_iter_is_key_str_map,
};


TAG_map_arg_t is_key_int_map =
{
	.iter_is_key_int =  TAG_iter_is_key_int_map,
};


TAG_map_arg_t get_iter_str_map =
{
	.iter_get_key_str =  TAG_iter_get_key_str_map,
};


TAG_map_arg_t get_iter_int_map =
{
	.iter_get_key_int =  TAG_iter_get_key_int_map
};

TAG_map_arg_t get_iter_value_map =
{
	.iter_get_value = TAG_iter_get_value
};

TAG_map_arg_t get_size_map =
{
	.get_size = TAG_get_size
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
	map->iter = 0;
	map->iter_active = 0;
	map->scroll_iter = 0;
}

void TAG_map_print_entries(TAG_ptr_t ptr)
{
	TAG_map_t *map = ptr.MAP;
	uint64_t old_max = map->allocated;
	for(uint64_t k = 0; k < old_max; ++k)
	{
		struct TAG_mapelm *elm = &map->map[k];
		if(elm->hash != -1)
		{

			if(elm->type == TAG_MAPKEY_STR)
			{
				printf("[%ld] = %s<%ld>\n", k, elm->key.STR, elm->hash);
			}
			else
			{
				printf("[%ld] = %ld<%ld>\n", k, elm->key.INT, elm->hash);

			}
		}
		else
		{
			printf("[%ld] = empty\n", k );
		}
	}


}

struct TAG_mapelm *TAG_map_get_element(TAG_tag_t *tag, uint64_t i)
{
	TAG_map_t *map = tag->ptr.MAP;
	uint64_t c = 0;
	for(uint64_t j = 0; j < map->allocated; ++j)
	{
		if(map->map[j].hash != -1)
		{
			if(c == i)
			{
				return &map->map[j];
			}
			c++;
		}
	}
	return  NULL;
}

struct TAG_mapelm *TAG_map_get_element_scroll(TAG_tag_t *tag)
{
	TAG_map_t *map = tag->ptr.MAP;
	if (map->scroll_iter >= map->allocated)
	{
		//printf("done\n");
		
    	return NULL;
	}
	//printf("scroll iter %ld\n", map->scroll_iter);
	struct TAG_mapelm *elm = &map->map[map->scroll_iter];
	if(elm->hash == -1)
	{
		for(uint64_t j = map->scroll_iter; j < map->allocated; ++j)
		{
			//printf("[%ld]=%ld\n", j,map->map[j].hash);

			if(map->map[j].hash != -1)
			{
				map->scroll_iter = j;
				elm = &map->map[map->scroll_iter];
				break;
			}
		}
	}
	else
	{
		//TAG_map_print_entries(tag->ptr);
		//printf("get nothing %ld\n", elm->hash);

	}
	return elm;
}


void TAG_map_scroll_up(TAG_tag_t *tag)
{
	TAG_map_t *map = tag->ptr.MAP;
 	map->scroll_iter = (map->scroll_iter >= map->allocated)? map->scroll_iter: map->scroll_iter + 1;
	for(uint64_t j = map->scroll_iter; j < map->allocated; ++j)
	{
		if(map->map[j].hash != -1)
		{
			map->scroll_iter = j;
			return;
		}
	}

}

void TAG_map_scroll_down(TAG_tag_t *tag)
{
	TAG_map_t *map = tag->ptr.MAP;
 	map->scroll_iter = (map->scroll_iter >= map->allocated)? map->scroll_iter: map->scroll_iter - 1;

	for(int64_t j = map->scroll_iter; j >= 0; --j)
	{
		if(map->map[j].hash != -1)
		{
			map->scroll_iter = j;
			return;

		}
	}
}

void TAG_map_balance_remove(TAG_ptr_t ptr, uint64_t index)
{
	TAG_map_t *map = ptr.MAP;

	const uint64_t max = map->allocated;
	assert(index < max);
	struct TAG_mapelm *remptr = &map->map[index];

	uint64_t hole = index;
	uint64_t pos = (hole + 1) % max;



	map->count--;
	remptr->hash = -1;
	TAG_mapkey_free(remptr->key, remptr->type);
	remptr->type = 0;
	remptr->key.INT = 0;
	TAG_free(remptr->tag);
	remptr->tag = NULL;


	for(uint64_t i = 1; i < max; ++i)
	{



		struct TAG_mapelm *elm = &map->map[pos];
		if(elm->hash == -1)
			break;
		uint64_t base = elm->hash % max;


		if((pos > hole && (base <= hole || base > pos)) || (pos < hole && (base <= hole && base > pos)))
		{
			map->map[hole] = map->map[pos];
			map->map[pos].hash = -1;
			map->map[pos].type = 0;
			map->map[pos].tag = NULL;
			map->map[pos].key.INT = 0;
			hole = pos;
		}

		pos = (pos + 1) % max;
	}

}

//default functions


void TAG_map_free(TAG_ptr_t ptr)
{
	TAG_map_t *map = ptr.MAP;

	for(uint64_t i = 0; i < map->allocated; ++i)
	{
		struct TAG_mapelm *elm = &map->map[i];
		if(elm->hash != -1)
		{
			mapkey_vtable_list[elm->type].free(elm->key);
			TAG_free(elm->tag);

		}
	}
	free(map->map);
	free(map);
}

void TAG_map_print(TAG_ptr_t ptr, uint64_t tab)
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
				TAG_print_tab_index_str(elm->tag, tab + 1, elm->key.STR);

			}
			else
			{
				TAG_print_tab_index_int(elm->tag, tab + 1, elm->key.INT);

			}
		}
	}
}

TAG_ptr_t TAG_map_copy(TAG_ptr_t ptr)
{
	TAG_map_t *map = ptr.MAP;
	uint64_t size = map->allocated;
	struct TAG_mapelm elms[size];
	memset(elms, 0, sizeof(elms));
	for(uint64_t i = 0; i < size; ++i)
	{
		struct TAG_mapelm tmp = map->map[i];
		if(tmp.hash == -1)
		{
			elms[i].hash = -1;
			continue;
		}
		struct TAG_mapelm elm_copy = tmp;
		elm_copy.key = TAG_mapkey_init(tmp.key, tmp.type);
		elm_copy.tag = TAG_copy(elm_copy.tag);
		//TAG_print(elm_copy.tag);
		elms[i] = elm_copy;
	}
	TAG_map_t *new = calloc(1, sizeof(TAG_map_t));
	assert(map);
	new->allocated = map->allocated;
	new->count = map->count;
	new->map = calloc(map->allocated, sizeof(struct TAG_mapelm));

	for(uint64_t j = 0; j < size; ++j)
	{

		new->map[j] = elms[j];
	}

	TAG_ptr_t rptr;
	rptr.MAP = new;



	return rptr;
}




//tag functions



static TAG_ptr_t TAG_init_map(void)
{
	TAG_map_t *map = calloc(1, sizeof(TAG_map_t));
	assert(map);
	map->allocated = 8;
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
			if(tag == NULL)
				return true;
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
					if(tag == NULL)
					{
						TAG_map_balance_remove(ptr, start);




						//TAG_map_realloc(ptr);
						//TAG_map_print_entries(ptr);
						return true;
					}
					else
					{
						assert(0 && "cannot replace element");
					}
				}
			}
			start = (start + 1) % map->allocated;
		}
		else
		{
			start = (start + 1) % map->allocated;
		}
	}
	if(tag == NULL)
		return true;
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
			//printf("tmp hash == -1 \n");
			return NULL;
		}
		else
		{
			start = (start + 1) %  map->allocated;
		}
	}
	//printf("got nothing\n");
	return NULL;

}



//string key behaviour

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
	//printf("%s\n", key);
	return TAG_get_key_map(ptr->ptr, mapkey, TAG_MAPKEY_STR);
}

static bool TAG_remove_key_string_map(TAG_tag_t *ptr, char *key)
{

	TAG_map_t *map = ptr->ptr.MAP;
	union TAG_mapkey mapkey;
	mapkey.STR = key;
	//printf("remove %s\n", key);
	assert(map->iter_active == false);
	return TAG_set_key_map(ptr->ptr, mapkey, TAG_MAPKEY_STR, NULL);

}

//int key behaviour

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

static bool TAG_remove_key_int_map(TAG_tag_t *ptr, int64_t key)
{
	TAG_map_t *map = ptr->ptr.MAP;
	assert(map->iter_active == false);
	union TAG_mapkey mapkey;
	mapkey.INT = key;
	return TAG_set_key_map(ptr->ptr, mapkey, TAG_MAPKEY_INT, NULL);
}

static bool TAG_iter_start_map(TAG_tag_t *tag)
{
	TAG_map_t *map = tag->ptr.MAP;

	if(map->iter_active == true)
	{
		return false;
	}
	else
	{
		map->iter_active = true;
		return true;
	}
}

static bool TAG_iter_reset_map(TAG_tag_t *tag)
{
	TAG_map_t *map = tag->ptr.MAP;
	map->iter = 0;
	map->scroll_iter = 0;
	assert(map->iter_active == true && "iter active must be true");

	//TAG_map_print_entries(tag->ptr);
	return true;
}

static bool TAG_iter_up_map(TAG_tag_t *tag)
{
	TAG_map_t *map = tag->ptr.MAP;
	assert(map->iter_active == true && "iter active must be true");

	map->iter = (map->iter < map->count)? map->iter + 1: map->iter;
	TAG_map_scroll_up(tag);
	return true;
}

static bool TAG_iter_down_map(TAG_tag_t *tag)
{
	TAG_map_t *map = tag->ptr.MAP;
	assert(map->iter_active == true && "iter active must be true");

	map->iter = (map->iter > 0)? map->iter - 1: 0;
	TAG_map_scroll_down(tag);

	return true;
}

static bool TAG_iter_is_end_map(TAG_tag_t *tag)
{
	TAG_map_t *map = tag->ptr.MAP;
	assert(map->iter_active == true && "iter active must be true");

	return (map->iter == map->count)? true : false;

}

static bool TAG_iter_end_map(TAG_tag_t *tag)
{
	TAG_map_t *map = tag->ptr.MAP;

	map->iter = 0;
	map->scroll_iter = 0;
	map->iter_active = false;
	return true;
}

static bool TAG_iter_is_key_str_map(TAG_tag_t *tag)
{
	TAG_map_t *map = tag->ptr.MAP;
	assert(map->iter_active == true && "iter active must be true");

	struct TAG_mapelm *elm = TAG_map_get_element_scroll(tag);
	if(elm == NULL)
		return false;
	if(elm->type == TAG_MAPKEY_STR)
	{
		return true;
	}
	return false;
}

static bool TAG_iter_is_key_int_map(TAG_tag_t *tag)
{
	TAG_map_t *map = tag->ptr.MAP;
	assert(map->iter_active == true && "iter active must be true");

	struct TAG_mapelm *elm = TAG_map_get_element_scroll(tag);

	if(elm == NULL)
		return false;
	if(elm->type == TAG_MAPKEY_INT)
	{
		return true;
	}
	return false;
}

static char *TAG_iter_get_key_str_map(TAG_tag_t *tag)
{
	TAG_map_t *map = tag->ptr.MAP;
	assert(map->iter_active == true && "iter active must be true");

	struct TAG_mapelm *elm = TAG_map_get_element_scroll(tag);

	if(elm == NULL)
		return NULL;
	if(elm->type == TAG_MAPKEY_STR)
	{
		//dangerous let's trust the user
		return elm->key.STR;
	}
	return NULL;
}

static int64_t TAG_iter_get_key_int_map(TAG_tag_t *tag)
{
	TAG_map_t *map = tag->ptr.MAP;
	assert(map->iter_active == true && "iter active must be true");

	struct TAG_mapelm *elm = TAG_map_get_element_scroll(tag);
	if(elm == NULL)
		return INT64_MIN;

	if(elm->type == TAG_MAPKEY_INT)
	{
		//printf("get elm %ld\n", elm->key.INT);

		return elm->key.INT;
	}
	return INT64_MIN;
}

static TAG_tag_t *TAG_iter_get_value(TAG_tag_t *tag)
{
	TAG_map_t *map = tag->ptr.MAP;
	assert(map->iter_active == true && "iter active must be true");
	//printf("\n\n");
	//TAG_map_print_entries(tag->ptr);

	struct TAG_mapelm *elm = TAG_map_get_element_scroll(tag);

	if(elm == NULL)
	{
		//TAG_map_print_entries(tag->ptr);
		assert(0);

		return NULL;

	}
	return elm->tag;
}

static uint64_t TAG_get_size(TAG_tag_t *tag)
{
	TAG_map_t *map = tag->ptr.MAP;
	return map->count;
}






TAG_prototype_vtable_t TAG_map_vtable =
{
	.free = TAG_map_free,
	.print = TAG_map_print,
	.copy = TAG_map_copy,
	.size = 19,
	.fn =
	{
		[TAG_FN_MAP_INIT].MAP = &init_map,
		[TAG_FN_MAP_SET_KEY_STRING].MAP = &set_key_string_map,
		[TAG_FN_MAP_GET_KEY_STRING].MAP = &get_key_string_map,
		[TAG_FN_MAP_REM_KEY_STRING].MAP = &rem_key_string_map,

		[TAG_FN_MAP_SET_KEY_INT].MAP = &set_key_int_map,
		[TAG_FN_MAP_GET_KEY_INT].MAP = &get_key_int_map,
		[TAG_FN_MAP_REM_KEY_INT].MAP = &rem_key_int_map,


		[TAG_FN_MAP_ITER_START].MAP = &iter_start_map,
		[TAG_FN_MAP_ITER_RESET].MAP = &iter_reset_map,
		[TAG_FN_MAP_ITER_UP].MAP = &iter_up_map,
		[TAG_FN_MAP_ITER_DOWN].MAP = &iter_down_map,
		[TAG_FN_MAP_ITER_IS_END].MAP = &iter_is_end_map,
		[TAG_FN_MAP_ITER_END].MAP = &iter_end_map,


		[TAG_FN_MAP_ITER_IS_KEY_INT].MAP = &is_key_int_map,
		[TAG_FN_MAP_ITER_IS_KEY_STR].MAP = &is_key_str_map,
		[TAG_FN_MAP_ITER_GET_KEY_STR].MAP = &get_iter_str_map,
		[TAG_FN_MAP_ITER_GET_KEY_INT].MAP = &get_iter_int_map,
		[TAG_FN_MAP_ITER_GET_VALUE].MAP = &get_iter_value_map,
		[TAG_FN_MAP_GET_SIZE].MAP = &get_size_map,
	}
};
