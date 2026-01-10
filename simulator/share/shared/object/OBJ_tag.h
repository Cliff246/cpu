#ifndef __WS_OBJ_TAG_HEADER__
#define __WS_OBJ_TAG_HEADER__

#define WS_OBJ_KEY_TAG_SIZE 20
#include "OBJ_value.h"

typedef struct WS_OBJ_tag
{

	char key[WS_OBJ_KEY_TAG_SIZE];
	OBJ_value_t *value;
}OBJ_tag_t;

#endif
