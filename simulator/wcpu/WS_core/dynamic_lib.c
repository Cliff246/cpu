#include "device_command_impl.h"
#include "hashmap.h"
#include "dynamic_lib.h"

#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include <errno.h>

#include <limits.h>
#include <unistd.h>
#include <libgen.h>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

WS_module_t *WS_global_module_list[WS_GLOBAL_MODULE_LIST_MAX_SIZE] = {NULL};




void WS_add_default_flags_to_hashmap(WS_module_t *module)
{
	if(module->dev_desc->flag_table->tablesize < WS_DEV_FLAG_DEFAULT_COUNT)
	{
		fprintf(stderr, "%s does not have a properly sized hash table, expected > %d, got %d\n", module->dev_desc->dev_name,WS_DEV_FLAG_DEFAULT_COUNT,module->dev_desc->flag_table->tablesize );
		exit(EXIT_FAILURE);
	}
	for(int i = 0; i < WS_DEV_FLAG_DEFAULT_COUNT; ++i)
	{
		//WS_cmd_producer_print(&WS_flag_default_producers[i]);

		addto_hash_table(module->dev_desc->flag_table, WS_flag_default_producers[i].id, &WS_flag_default_producers[i]);

	}
}

static int WS_get_executable_path(char out[PATH_MAX])
{
	errno = 0;
	char path[PATH_MAX] = {0};

	uint32_t size = sizeof(path);

#ifdef __APPLE__
	char *temp = getcwd(path, size);
	if(errno != 0)
	{

		perror("get working directory failed");
		assert(0);

	}
#else
	size_t len = readlink("/proc/self/exe", path, size - 1);
	//printf("%s\n", path);
	if(len < 0)
		return -1;
	assert(dirname((char *)path));
	path[len] = '\0';

#endif
	strncpy(out, path, size);
	out[PATH_MAX - 1] = '\0';
	return 0;
}

static char *WS_module_resolve_path(const char *module_filename)
{

	char exe_dir[PATH_MAX] = {0};

	int result = WS_get_executable_path(exe_dir);

	if(result < 0)
	{
		assert(0);
	}

#ifdef __APPLE__
	const char *suffix = ".dylib";
#else
	const char *suffix = ".so";

#endif
	result = 0;

	char *out_dir = calloc(PATH_MAX + 1, sizeof(char));
	assert(out_dir);

	result = snprintf(out_dir, PATH_MAX, "%s/%s%s", exe_dir, module_filename, suffix);
	//printf("%s %s %s\n", out_dir, module_filename, suffix);
	if(result < 0)
	{
		free(out_dir);
		assert(0);
	}
	//printf("%s\n", exe_dir);
	return out_dir;
}


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


WS_module_t *WS_module_create(const char *filename)
{


	const int flags = RTLD_NOW;

	char *filepath = WS_module_resolve_path(filename);
	assert(filepath);

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

	WS_add_default_flags_to_hashmap(module);

	return module;
}
