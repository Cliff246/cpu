#include "module.h"
#include "dlfcn.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

WS_module_t *WS_global_module_list[WS_GLOBAL_MODULE_LIST_MAX_SIZE] = {NULL};


static void WS_module_list_fix(void)
{
	WS_module_t *temp[WS_GLOBAL_MODULE_LIST_MAX_SIZE] = {0};
	size_t count = 0;
	for(int i = 0; i < WS_GLOBAL_MODULE_LIST_MAX_SIZE; ++i)
	{
		if(WS_global_module_list[i] != NULL)
		{
			temp[count++] = WS_global_module_list[i];
		}
	}

	memcpy(WS_global_module_list, temp, sizeof(WS_module_t*) * WS_GLOBAL_MODULE_LIST_MAX_SIZE);
}
static bool WS_module_list_insert_free(WS_module_t *module)
{
	for(int i = 0; i < WS_GLOBAL_MODULE_LIST_MAX_SIZE; ++i)
	{
		if(WS_global_module_list[i] == NULL)
		{
			WS_global_module_list[i] = module;
			return true;
		}
	}
	return false;
}

void WS_module_free(WS_module_t *module)
{
	assert(module->dl_ptr != NULL);
	for(int i = 0; i < WS_GLOBAL_MODULE_LIST_MAX_SIZE; ++i)
	{
		if(module->dl_ptr == WS_global_module_list[i]->dl_ptr)
		{
			WS_global_module_list[i] = NULL;
		}
	}
	WS_module_list_fix();

	dlclose(module->dl_ptr);
	free(module->filepath);

	free(module);
}


WS_module_t *WS_module_create(const char *filepath)
{


	const int flags = RTLD_NOW;

	void *handle = dlopen(filepath, flags);



	if(!handle)
	{
		fprintf(stderr, "dlopen failed at %s\n", dlerror());
		assert(0);
	}
	DEVICE_EXPORT WS_dev_desc_t *(*get_desc)(void) = dlsym(handle, "WS_get_dev_desc");

	if(!get_desc)
	{

		fprintf(stderr, "dlsym failed at %s\n", dlerror());
		assert(0);
	}

	WS_module_t *module = calloc(1, sizeof(WS_module_t));
	assert(module);
	assert(WS_module_list_insert_free(module));

	module->filepath = strdup(filepath);
	assert(module->filepath);

	module->dl_ptr = handle;
	module->flags = flags;
	//this is not a safe reference to the device_description, but all device_descriptions should be static
	module->dev_desc = get_desc();

	assert(module->dev_desc);
	return module;
}