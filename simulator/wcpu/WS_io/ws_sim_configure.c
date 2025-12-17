#include "ws_sim_configure.h"
#include "loader.h"
#include "module.h"
#include "token.h"
#include "device_command_impl.h"
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>



WS_config_entry_t *WS_create_config_entry(WS_module_t *module, toklex_t *token_lex)
{
	assert(token_lex);
	assert(module);

	WS_config_entry_t *entry = calloc(1, sizeof(WS_config_entry_t));
	assert(entry);
	assert(module->dev_desc->vtable->stringfy);
	entry->cmd = module->dev_desc->vtable->stringfy(token_lex);
	return entry;
}

void WS_append_new_config_module_container_entry(WS_config_module_container_t *container, WS_config_entry_t *entry)
{
	container->entry_list = realloc_safe(container->entry_list, container->entry_list_count + 1, sizeof(WS_config_entry_t**));
	container->entry_list[container->entry_list_count++] = entry;
	return;


}


static void load_module_containers_config_file(WS_config_file_t *file)
{
	open_sourcefile(file->srcfile);




	WS_config_module_container_t *container = NULL;

	bool has_container = false;



	while(true)
	{
		if(get_isend_sourcefile(file->srcfile))
		{
			break;
		}
		char *line = readline_sourcefile(file->srcfile);
		toklex_t *toklex = lex_string(line);
		tok_t *tok = peek_toklex(toklex);
		if(tok->type == TOK_STRING)
		{
			if(has_container)
			{
				WS_append_config_file_module_container(file, container);
			}

			container = WS_create_config_module_container(tok->token);
			has_container = true;
		}
		else
		{
			WS_config_entry_t *entry =  WS_create_config_entry(container->module, toklex);
			WS_append_new_config_module_container_entry(container, entry);
		}



		//print_toklex(toklex);
		//WS_config_module_container_t *container  =WS_create_config_module_container(line);
		//
		free_toklex(toklex);
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