#ifndef __WS_SIM_CONFIGURE_HEADER__
#define __WS_SIM_CONFIGURE_HEADER__

#include "loader.h"
#include "module.h"
#include "token.h"
#include "device_command.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


typedef struct WS_config_element
{


	tok_t *key;
	tok_t *value;

}WS_cfg_elem_t;

WS_cfg_elem_t WS_cfg_create_element(tok_t **buf, size_t size);
void WS_cfg_print_element(WS_cfg_elem_t *elem);
typedef struct WS_config_body
{
	WS_dev_cmd_collection_t *collection;
	WS_cfg_elem_t *elements;
	size_t size;
}WS_cfg_body_t;


//this is big copy i suppose
void WS_cfg_append_body(WS_cfg_body_t *body, WS_cfg_elem_t elem);
WS_cfg_body_t *WS_cfg_create_body(void);
void WS_cfg_free_body(WS_cfg_body_t *body);
void WS_cfg_print_body(WS_cfg_body_t *body);
void WS_cfg_collect_body(WS_cfg_body_t *body);
typedef struct WS_config_header
{
	WS_module_t *module;
	WS_dev_cmd_t **cmds;

	WS_cfg_body_t **bodies;
	size_t size;

}WS_cfg_hdr_t;

WS_cfg_hdr_t *WS_cfg_create_header(WS_module_t *module);
void WS_cfg_append_header(WS_cfg_hdr_t *header, WS_cfg_body_t *body);
void WS_cfg_free_header(WS_cfg_hdr_t *header);
void WS_cfg_collect_header(WS_cfg_hdr_t *header);

typedef struct WS_config_file
{
	sourcefile_t *srcfile;


	WS_cfg_hdr_t **headers;
	size_t size;
}WS_cfg_file_t;

void WS_cfg_append_file(WS_cfg_file_t *file, WS_cfg_hdr_t *header);
WS_cfg_file_t *WS_cfg_create_file(const char *path);
//does not free module
void WS_cfg_free_file(WS_cfg_file_t *file);

#endif