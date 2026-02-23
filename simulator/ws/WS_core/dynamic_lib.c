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

p_hashtable_t WS_dynamic_lib_table = NULL;




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


void WS_dynamic_lib_startup(void)
{
	static bool run = false;
	if(run == true)
		return;
	WS_dynamic_lib_table = new_hash_table(WS_GLOBAL_DL_LIST_MAX_SIZE, WS_dynamic_lib_free);
	run = true;
}

void WS_dynamic_lib_free(void *ptr)
{
	assert(0);
}

WS_dynamic_lib_t *WS_dynamic_lib_get(const char *filename)
{

	WS_dynamic_lib_startup();
	const int flags = RTLD_NOW;

	WS_dynamic_lib_t *try = (WS_dynamic_lib_t *) getdata_from_hash_table(WS_dynamic_lib_table, filename);
	if(try != NULL)
	{
		return try;
	}

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

	dl->filepath = strdup(filepath);
	assert(dl->filepath);

	dl->dl_ptr = handle;
	dl->flags = flags;
	//this is not a safe reference to the device_description, but all device_descriptions should be static
	dl->dev_desc = get_desc();
	assert(dl->dev_desc);
	printf("added %s\n", dl->dev_desc->dev_name);
	addto_hash_table(WS_dynamic_lib_table, filename, dl);
	return dl;
}
