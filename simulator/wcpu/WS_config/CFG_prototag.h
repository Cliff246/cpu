#ifndef __WS_CFG_TAG_HEADER__
#define __WS_CFG_TAG_HEADER__

#include "TAG_tag.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct WS_CFG_prototag
{
	char *key;
	TAG_tag_t *tag;
}CFG_prototag_t;

CFG_prototag_t *CFG_init_prototag_empty(char *key);

CFG_prototag_t *CFG_init_prototag_string(char *key, char *value);
CFG_prototag_t *CFG_init_prototag_int(char *key, int64_t integer);
CFG_prototag_t *CFG_init_prototag_list_string(char *key, char **str, uint64_t count);
CFG_prototag_t *CFG_init_prototag_list_ints(char *key, int64_t *integers, uint64_t count);

CFG_prototag_t *CFG_init_prototag_list(char *key, TAG_tag_t *tags, uint64_t count);
CFG_prototag_t *CFG_init_prototag_bool(char *key, bool boolean);

void CFG_free_prototag(CFG_prototag_t *prototag);


#endif