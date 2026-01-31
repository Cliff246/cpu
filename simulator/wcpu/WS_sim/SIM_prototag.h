#ifndef __WS_SIM_TAG_HEADER__
#define __WS_SIM_TAG_HEADER__

#include "SIM_commons.h"
#include "TAG_tag.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct WS_SIM_prototag
{
	char *key;
	TAG_tag_t *tag;
}SIM_prototag_t;

SIM_prototag_t *SIM_init_prototag_empty(char *key);

SIM_prototag_t *SIM_init_prototag_string(char *key, char *value);
SIM_prototag_t *SIM_init_prototag_int(char *key, int64_t integer);
SIM_prototag_t *SIM_init_prototag_list_string(char *key, char **str, uint64_t count);

SIM_prototag_t *SIM_init_prototag_list(char *key, TAG_tag_t *tags, uint64_t count);
SIM_prototag_t *SIM_init_prototag_bool(char *key, bool boolean);

void SIM_free_prototag(SIM_prototag_t *prototag);


#endif