#include "device.h"
#include "device_list.h"
#include "device_description.h"
#include "device_command.h"
#include "ram_device_config_setting.h"

#include <stdlib.h>
#include <assert.h>


WS_dev_cmd_flag_apply_fn device_ram_producer_functions[DEVICE_RAM_CMD_OPTIONS_COUNT] =
{
	DEVICE_RAM_CMD_OPTIONS_LIST(DEVICE_RAM_CMD_PRODUCER_FUNC_ARRAY)
};

char *device_ram_producer_names[DEVICE_RAM_CMD_OPTIONS_COUNT] =
{
	DEVICE_RAM_CMD_OPTIONS_LIST(DEVICE_RAM_CMD_PRODUCER_NAME_ARRAY)
};

WS_dev_cmd_flag_type_t device_ram_producer_types[DEVICE_RAM_CMD_OPTIONS_COUNT] =
{
	DEVICE_RAM_CMD_OPTIONS_LIST(DEVICE_RAM_CMD_PRODUCER_TYPE_ARRAY)

};


bool device_cmd_option_producer_FILENAME(WS_dev_t *device, WS_dev_cmd_flag_t *flag)
{

}

bool device_cmd_option_producer_SIZE(WS_dev_t *device, WS_dev_cmd_flag_t *flag)
{

}

bool device_cmd_option_producer_START(WS_dev_t *device, WS_dev_cmd_flag_t *flag)
{

}

bool device_cmd_option_producer_PRINT_CONTENT(WS_dev_t *device, WS_dev_cmd_flag_t *flag)
{

}

bool device_cmd_option_producer_RESET(WS_dev_t *device, WS_dev_cmd_flag_t *flag)
{

}
