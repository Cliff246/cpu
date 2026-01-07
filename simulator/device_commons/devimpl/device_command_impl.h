#ifndef __DEVICE_COMMAND_IMPL_HEADER__
#define __DEVICE_COMMAND_IMPL_HEADER__

#include "device.h"
#include "device_command.h"
#include <stdbool.h>
#include "token.h"
#include "module_description.h"

bool WS_device_cmd(WS_dev_t *device, WS_dev_desc_t *desc, WS_dev_cmd_t *cmd);

void WS_cmd_free(WS_dev_cmd_t *cmd);



void WS_cmd_producer_free(void *producer);
void WS_cmd_producer_print(WS_dev_flag_producer_t *producer);
WS_dev_flag_producer_t *WS_cmd_flag_producer_create(char *string, WS_dev_flag_type_t type, WS_dev_flag_apply_fn fn);

void WS_cmd_collection_append(WS_dev_cmd_collection_t *col, WS_dev_flag_t *flag);
void WS_cmd_collection_free(WS_dev_cmd_collection_t *col);

WS_dev_flag_t *WS_dev_flag_create(tok_t *key, tok_t *value);
WS_dev_cmd_t *WS_dev_cmd_create(WS_dev_cmd_collection_t *collection, WS_dev_desc_t *desc);
WS_dev_cmd_collection_t *WS_cmd_collection_create(void);

extern void (*WS_flag_value_free_list[])(WS_dev_flag_value_t *value);
extern char *WS_flag_type_strings[];
extern WS_dev_flag_producer_t WS_flag_default_producers[];
#endif