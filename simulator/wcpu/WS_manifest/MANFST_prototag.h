#ifndef __MANFST_PROTOTAG_HEADER__
#define __MANFST_PROTOTAG_HEADER__

#include "TAG_tag.h"
#include <stdint.h>
#include <stdbool.h>



typedef struct MANFST_prototag
{
	uint64_t key_size, name_size;
	char *buffer;

	TAG_tag_t *tag;
}MANFST_prototag_t;



void MANFST_get_prototag_key(MANFST_prototag_t *prototag, char *buffer, uint64_t length);
void MANFST_get_prototag_name(MANFST_prototag_t *prototag, char *buffer, uint64_t length);

MANFST_prototag_t *MANFST_init_prototag_empty(char *key, char *name);
MANFST_prototag_t *MANFST_init_prototag_string(char *key, char *name, char *value);
MANFST_prototag_t *MANFST_init_prototag_int(char *key, char *name, int64_t integer);
MANFST_prototag_t *MANFST_init_prototag_list_string(char *key, char *name, char **str, uint64_t count);
MANFST_prototag_t *MANFST_init_prototag_list_ints(char *key, char *name, int64_t *integers, uint64_t count);
MANFST_prototag_t *MANFST_init_prototag_list_bools(char *key, char *name, bool *booleans, uint64_t count);
MANFST_prototag_t *MANFST_init_prototag_list(char *key, char *name, TAG_tag_t *tags, uint64_t count);
MANFST_prototag_t *MANFST_init_prototag_bool(char *key,char *name, bool boolean);

void MANFST_free_prototag(MANFST_prototag_t *prototag);
void MANFST_print_prototag(MANFST_prototag_t *prototag);

#endif