#ifndef __MODULE_HEADER__
#define __MODULE_HEADER__

#include "device.h"
#include "device_description.h"



typedef struct wcpu_simulator_module
{
	char *filepath;
	WS_dev_desc_t *dev_desc;
	void *dl_ptr;
	int flags;

}WS_module_t;

WS_module_t *WS_module_create(const char *filepath);
void WS_module_free(WS_module_t *module);


#endif