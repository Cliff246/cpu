#ifndef __SCENE_COMMONS_HEADER__
#define __SCENE_COMMONS_HEADER__

#include "TAG_tag.h"

#include <stdint.h>


int64_t SCENE_get_size_tags_map(TAG_tag_t *map);
int64_t SCENE_get_all_tags_map(TAG_tag_t *map, TAG_tag_t **buffer, uint64_t bsize);


#endif