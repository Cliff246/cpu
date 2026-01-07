#ifndef __WS_OBJECT_FLAG_HEADER__
#define __WS_OBJECT_FLAG_HEADER__

#include "WS_OBJ_object.h"
#include "WS_OBJ_value.h"
#include "commons.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define WS_OBJ_KEY_FLAG_SIZE 20


//goes into a hashmap is index and then executed via the function in it


typedef struct WS_OBJ_flag
{
	char key[WS_OBJ_KEY_FLAG_SIZE];

	OBJ_value_t *value;
	
}OBJ_flag_t;












#endif
