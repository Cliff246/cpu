#include "ws_sim_configure.h"
#include "loader.h"
#include "module.h"
#include <stdlib.h>
#include <assert.h>

static void load_module_containers_config_file(WS_config_file_t *file)
{
	open_sourcefile(file->srcfile);

	char *line;

	while(line = readline_sourcefile(file->srcfile))
	{
		printf("<%s>\n", line);

		WS_config_module_container_t *container  =WS_create_config_module_container(line);
		WS_append_config_file_module_container(file, container);

		free(line);
	}


	close_sourcefile(file->srcfile);

}

WS_config_module_container_t *WS_create_config_module_container(char *path)
{
	WS_config_module_container_t *container =  calloc(1, sizeof(WS_config_module_container_t));

	WS_module_t *module = WS_module_create(path);

	container->module = module;




	return container;
}


//so unsafe lol
void WS_append_config_file_module_container(WS_config_file_t *file, WS_config_module_container_t *mc)
{
	file->module_container_list = realloc_safe(file->module_container_list, file->module_container_list_count + 1, sizeof( WS_config_module_container_t**));
	file->module_container_list[file->module_container_list_count ++] = mc;
	return;
}

WS_config_file_t *WS_create_config_file(const char *path)
{



	WS_config_file_t *config = calloc(1, sizeof(WS_config_file_t));

	sourcefile_t *sf = create_sourcefile((char *)path);

	assert(get_canopen_sourcefile(sf));

	config->srcfile = sf;


	load_module_containers_config_file(config);









	return config;
}