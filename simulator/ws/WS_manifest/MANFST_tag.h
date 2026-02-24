#ifndef __MANFST_TAG_HEADER__
#define __MANFST_TAG_HEADER__


#include "TAG_tag.h"
#include <stdint.h>
#include <stdbool.h>







TAG_tag_t *MANFST_init_tag_string(char *value);
TAG_tag_t *MANFST_init_tag_int(int64_t integer);
TAG_tag_t *MANFST_init_tag_list_string(char **str, uint64_t count);
TAG_tag_t *MANFST_init_tag_list_ints(int64_t *integers, uint64_t count);
TAG_tag_t *MANFST_init_tag_list_bools(bool *booleans, uint64_t count);
TAG_tag_t *MANFST_init_tag_list();
TAG_tag_t *MANFST_init_tag_map();
TAG_tag_t *MANFST_init_tag_bool(bool boolean);

#endif