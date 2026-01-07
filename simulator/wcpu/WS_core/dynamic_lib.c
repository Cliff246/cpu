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
WS_dynamic_lib_t *WS_global_dl_list[WS_GLOBAL_DL_LIST_MAX_SIZE] = {NULL};




void WS_add_default_flags_to_hashmap(WS_dynamic_lib_t *dl)
{

	/*if(dl->dev_desc->flag_table->tablesize < WS_DEV_FLAG_DEFAULT_COUNT)
	{
		fprintf(stderr, "%s does not have a properly sized hash table, expected > %d, got %d\n", dl->dev_desc->dev_name,WS_DEV_FLAG_DEFAULT_COUNT, dl->dev_desc->flag_table->tablesize );
		exit(EXIT_FAILURE);
	}
	for(int i = 0; i < WS_DEV_FLAG_DEFAULT_COUNT; ++i)
	{
		//WS_cmd_producer_print(&WS_flag_default_producers[i]);

		addto_hash_table(dl->dev_desc->flag_table, WS_flag_default_producers[i].id, &WS_flag_default_producers[i]);

	}
	*/
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
static char *WS_dynamic_lib_resolve_path(const char *filename)
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

	result = snprintf(out_dir, PATH_MAX, "%s/%s%s", exe_dir, filename, suffix);
	//printf("%s %s %s\n", out_dir, module_filename, suffix);
	if(result < 0)
	{
		free(out_dir);
		assert(0);
	}
	//printf("%s\n", exe_dir);
	return out_dir;
}


static void WS_dynamic_lib_list_fix(void)
{
	WS_dynamic_lib_t *temp[WS_GLOBAL_DL_LIST_MAX_SIZE] = {0};
	size_t count = 0;
	for(int i = 0; i < WS_GLOBAL_DL_LIST_MAX_SIZE; ++i)
	{
		if(WS_global_dl_list[i] != NULL)
		{
			temp[count++] = WS_global_dl_list[i];
		}
	}

	memcpy(WS_global_dl_list, temp, sizeof(WS_dynamic_lib_t*) * WS_GLOBAL_DL_LIST_MAX_SIZE);
}

static bool WS_dynamic_lib_list_insert_free(WS_dynamic_lib_t *dl)
{
	for(int i = 0; i < WS_GLOBAL_DL_LIST_MAX_SIZE; ++i)
	{
		if(WS_global_dl_list[i] == NULL)
		{
			WS_global_dl_list[i] = dl;
			return true;
		}
	}
	return false;
}
void WS_dynamic_lib_free(WS_dynamic_lib_t *dl)
{
	assert(dl->dl_ptr != NULL);
	for(int i = 0; i < WS_GLOBAL_DL_LIST_MAX_SIZE; ++i)
	{
		if(dl->dl_ptr == WS_global_dl_list[i]->dl_ptr)
		{
			WS_global_dl_list[i] = NULL;
		}
	}
	WS_dynamic_lib_list_fix();

	dlclose(dl->dl_ptr);
	free(dl->filepath);

	free(dl);
}

WS_dynamic_lib_t *WS_dynamic_lib_create(const char *filename)
{


	const int flags = RTLD_NOW;

	char *filepath = WS_dynamic_lib_resolve_path(filename);
	assert(filepath);

	void *handle = dlopen(filepath, flags);



	if(!handle)
	{
		fprintf(stderr, "dlopen failed at %s\n", dlerror());
		assert(0);
	}
	DEVICE_EXPORT MOD_description_t *(*get_desc)(void) = dlsym(handle, "MOD_get_dev_desc");

	if(!get_desc)
	{

		fprintf(stderr, "dlsym failed at %s\n", dlerror());
		assert(0);
	}

	WS_dynamic_lib_t *dl = calloc(1, sizeof(WS_dynamic_lib_t));
	assert(dl);
	assert(WS_dynamic_lib_list_insert_free(dl));

	dl->filepath = strdup(filepath);
	assert(dl->filepath);

	dl->dl_ptr = handle;
	dl->flags = flags;
	//this is not a safe reference to the device_description, but all device_descriptions should be static
	dl->dev_desc = get_desc();
	assert(dl->dev_desc);

	WS_add_default_flags_to_hashmap(dl);

	return dl;
}
