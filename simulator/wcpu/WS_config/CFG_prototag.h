#ifndef __CFG_PROTOTAG_HEADER__
#define __CFG_PROTOTAG_HEADER__

#include "TAG_tag.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct CFG_prototag
{
	uint64_t namespace;
	char *key;
	char *name;
	TAG_tag_t *tag;
}CFG_prototag_t;

CFG_prototag_t *CFG_init_prototag_empty(char *key, char *name, uint64_t namespace);

CFG_prototag_t *CFG_init_prototag_string(char *key, char *name, uint64_t namespace, char *value);
CFG_prototag_t *CFG_init_prototag_int(char *key, char *name, uint64_t namespace, int64_t integer);
CFG_prototag_t *CFG_init_prototag_list_string(char *key, char *name, uint64_t namespace, char **str, uint64_t count);
CFG_prototag_t *CFG_init_prototag_list_ints(char *key, char *name, uint64_t namespace, int64_t *integers, uint64_t count);
CFG_prototag_t *CFG_init_prototag_list_bools(char *key, char *name, uint64_t namespace, bool *booleans, uint64_t count);

CFG_prototag_t *CFG_init_prototag_list(char *key, char *name, uint64_t namespace, TAG_tag_t *tags, uint64_t count);
CFG_prototag_t *CFG_init_prototag_bool(char *key,char *name, uint64_t namespace, bool boolean);

void CFG_free_prototag(CFG_prototag_t *prototag);
void CFG_print_prototag(CFG_prototag_t *prototag);

#endif