#ifndef __DEVICE_RAM_EMULATOR_CONFIG_SETTING_HEADER__
#define __DEVICE_RAM_EMULATOR_CONFIG_SETTING_HEADER__

#include "device_list.h"
#include "commons.h"
#include "device_description.h"
#include "device_command.h"
#include <stdint.h>
#include <stdbool.h>

#define DEVICE_RAM_CMD_OPTIONS_LIST(X)					\
	X(FILENAME, WS_DEV_CMD_FLAG_TYPE_STRING)			\
	X(SIZE, WS_DEV_CMD_FLAG_TYPE_UINT)					\
	X(START, WS_DEV_CMD_FLAG_TYPE_UINT)					\
	X(PRINT_CONTENT, WS_DEV_CMD_FLAG_TYPE_BOOL)			\
	X(RESET, WS_DEV_CMD_FLAG_TYPE_BOOL)					\

#define DEVICE_RAM_CMD_OPTIONS_NAME(X) DEV_RAM_CMD_OPTIONS_FLAG_ ## X

#define DEVICE_RAM_CMD_OPTIONMS_ENUM(X, Y) DEVICE_RAM_CMD_OPTIONS_NAME(X),

typedef enum
{
	DEVICE_RAM_CMD_OPTIONS_LIST(DEVICE_RAM_CMD_OPTIONMS_ENUM)

}dev_ram_cmd_options_t;

#define DEVICE_RAM_CMD_OPTIONS_COUNT (0 + DEVICE_RAM_CMD_OPTIONS_LIST(COUNTER2D))
#define DEVICE_RAM_CMD_PRODUCER_NAME(X) device_cmd_option_producer_ ## X

#define DEVICE_RAM_CMD_PRODUCER(X, Y) bool DEVICE_RAM_CMD_PRODUCER_NAME(X)(WS_dev_t *device, WS_dev_cmd_flag_t *flag);
DEVICE_RAM_CMD_OPTIONS_LIST(DEVICE_RAM_CMD_PRODUCER)
#define DEVICE_RAM_CMD_PRODUCER_FUNC_ARRAY(X,Y)[DEVICE_RAM_CMD_OPTIONS_NAME(X)] = DEVICE_RAM_CMD_PRODUCER_NAME(X),
#define DEVICE_RAM_CMD_PRODUCER_NAME_ARRAY(X,Y) [DEVICE_RAM_CMD_OPTIONS_NAME(X)] = #X,
#define DEVICE_RAM_CMD_PRODUCER_TYPE_ARRAY(X,Y) [DEVICE_RAM_CMD_OPTIONS_NAME(X)] = Y,

extern WS_dev_cmd_flag_apply_fn device_ram_producer_functions[DEVICE_RAM_CMD_OPTIONS_COUNT];
extern char *device_ram_producer_names[DEVICE_RAM_CMD_OPTIONS_COUNT];
extern WS_dev_cmd_flag_type_t device_ram_producer_types[DEVICE_RAM_CMD_OPTIONS_COUNT];

typedef struct device_ram_config_setting
{
	//file name flag
	char *filename;
	//size flag
	uint64_t size;
	uint64_t start;
	//size print content flag
	bool print_content;
	bool settings[DEVICE_RAM_CMD_OPTIONS_COUNT];
}dev_ram_config_setting_t;

void *device_init_config_setting_ram(WS_dev_desc_t *desc);
void device_free_config_setting_ram(void *ptr);



#endif