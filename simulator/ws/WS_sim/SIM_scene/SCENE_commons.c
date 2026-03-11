#include "SCENE_commons.h"

#include "TAG_map.h"
#include "TAG_tag.h"
#include <assert.h>

//TODO
int64_t SCENE_get_size_tags_map(TAG_tag_t *map)
{
	assert(map->type == TAG_MAP);

	TAG_argptr_t get_size = TAG_get_fn(TAG_MAP, TAG_FN_MAP_GET_SIZE);
	int64_t size = get_size.MAP->get_size(map);

	return size;

}

int64_t SCENE_get_all_tags_map(TAG_tag_t *map, TAG_tag_t **buffer, uint64_t bsize)
{
	assert(map->type == TAG_MAP);

	TAG_argptr_t get_size = TAG_get_fn(TAG_MAP, TAG_FN_MAP_GET_SIZE);
	TAG_argptr_t start_iter = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_START);
	TAG_argptr_t reset_iter = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_RESET);
	TAG_argptr_t up_iter = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_UP);
	TAG_argptr_t is_end_iter = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_IS_END);
	TAG_argptr_t end_iter = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_END);
	TAG_argptr_t get_value = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_GET_VALUE);

	uint64_t size = get_size.MAP->get_size(map);

	assert(size <= bsize && bsize > 0);

	start_iter.MAP->iter_start(map);
	reset_iter.MAP->iter_reset(map);

	uint64_t count = 0;
	while(is_end_iter.MAP->iter_is_end(map) == false)
	{
		TAG_tag_t *tag = get_value.MAP->iter_get_value(map);
		assert(tag != NULL);
			buffer[count++] = tag;
		assert(count <= bsize);
		up_iter.MAP->iter_up(map);
	}
	end_iter.MAP->iter_end(map);


	return count;
}