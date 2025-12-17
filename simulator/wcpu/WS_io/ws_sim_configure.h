#ifndef __WS_SIM_CONFIGURE_HEADER__
#define __WS_SIM_CONFIGURE_HEADER__

#include "loader.h"
#include "module.h"
#include "token.h"
#include "device_command.h"
#include <stdlib.h>

typedef struct wcpu_simulator_config_entry
{


	WS_dev_cmd_t *cmd;
	
}WS_config_entry_t;

WS_config_entry_t *WS_create_config_entry(WS_module_t *module, toklex_t *token_lex);


typedef struct wcpu_simulator_config_container
{


	WS_module_t *module;


	WS_config_entry_t **entry_list;
	size_t entry_list_count;

}WS_config_module_container_t;

WS_config_module_container_t *WS_create_config_module_container(char *path);
void WS_append_new_config_module_container_entry(WS_config_module_container_t *container, WS_config_entry_t *entry);


typedef struct wcpu_simulator_configure_file
{
	sourcefile_t *srcfile;


	WS_config_module_container_t **module_container_list;
	size_t module_container_list_count;

}WS_config_file_t;

void WS_append_config_file_module_container(WS_config_file_t *file, WS_config_module_container_t *mc);
WS_config_file_t *WS_create_config_file(const char *path);

#endif