#ifndef __DYLIB_HEADER__
#define __DYLIB_HEADER__

#include "MOD_description.h"
#include "hashmap.h"
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

typedef struct WS_dynamic_lib
{
	char *filepath;
	MOD_description_t *dev_desc;
	void *dl_ptr;
	int flags;

}WS_dynamic_lib_t;

extern p_hashtable_t WS_dynamic_lib_table;

#define WS_GLOBAL_DL_LIST_MAX_SIZE 100

static int WS_get_executable_path(char out[PATH_MAX]);
static char *WS_dynamic_lib_resolve_path(const char *filename);
void WS_dynamic_lib_startup(void);

void WS_dynamic_lib_free(void *ptr);
WS_dynamic_lib_t *WS_dynamic_lib_get(const char *filename);


#endif
