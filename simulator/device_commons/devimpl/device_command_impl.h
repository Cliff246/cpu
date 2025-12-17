#ifndef __DEVICE_COMMAND_IMPL_HEADER__
#define __DEVICE_COMMAND_IMPL_HEADER__

#include "device.h"
#include "device_command.h"
#include "device_description.h"


bool WS_device_cmd(WS_dev_t *device, WS_dev_cmd_t *cmd);

void WS_device_cmd_free(WS_dev_cmd_t *cmd);

void WS_cmd_producer_free(void *producer);

WS_dev_cmd_flag_producer_t *WS_cmd_flag_producer_create(char *string, WS_dev_cmd_flag_type_t type, WS_dev_cmd_flag_apply_fn fn);

#endif