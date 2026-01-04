#ifndef __CORE_DLHND_HEADER__
#define __CORE_DLHND_HEADER__

#include "module_description.h"
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

typedef struct WS_CORE_dlhnd
{
	char *filepath;
	MOD_description_t *desc;
	void *dl_ptr;
	int flags;

}CORE_dlhnd_t;

/*

#define WS_GLOBAL_MODULE_LIST_MAX_SIZE 100
extern WS_module_t *WS_global_module_list[WS_GLOBAL_MODULE_LIST_MAX_SIZE];

static int WS_get_executable_path(char out[PATH_MAX]);
static char *WS_module_resolve_path(const char *module_filename);
WS_module_t *WS_module_create(const char *filename);
void WS_module_free(WS_module_t *module);

*/
#endif
