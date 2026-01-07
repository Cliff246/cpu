#ifndef __DYLIB_HEADER__
#define __DYLIB_HEADER__

#include "WS_MOD_description.h"
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

#define WS_GLOBAL_DL_LIST_MAX_SIZE 100
extern WS_dynamic_lib_t *WS_global_dl_list[WS_GLOBAL_DL_LIST_MAX_SIZE];

static int WS_get_executable_path(char out[PATH_MAX]);
static char *WS_dynamic_lib_resolve_path(const char *filename);
WS_dynamic_lib_t *WS_dynamic_lib_create(const char *filename);
void WS_dynamic_lib_free(WS_dynamic_lib_t *dl);


#endif
