#include "module.h"
#include "dlfcn.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>


void WS_module_free(WS_module_t *module)
{
	dlclose(module->dl_ptr);
	free(module->filepath);

	assert(0 && "this is not implemented but should call ");
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

	module->filepath = strdup(filepath);
	assert(module->filepath);

	module->dl_ptr = handle;
	module->flags = flags;
	//this is not a safe reference to the device_description, but all device_descriptions should be static
	module->dev_desc = get_desc();

	printf("%s\n", module->dev_desc->dev_name);
	assert(module->dev_desc);
	return module;
}