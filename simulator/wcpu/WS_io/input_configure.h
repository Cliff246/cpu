#ifndef __EMULATOR_CONFIGURE_HEADER__
#define __EMULATOR_CONFIGURE_HEADER__

#include "loader.h"
#include "device_list.h"
#include "device_description.h"
#include "token.h"
#include "device_command.h"
#include "input_parser.h"
#include <stdbool.h>


//used for the 'main' configuration type of the emulator config, either default(internal) or set(external)
typedef enum emuconfig_type
{
	INPUT_CONFIG_TYPE_INVAL,

	INPUT_CONFIG_TYPE_EXTERNAL_SOURCE,
	INPUT_CONFIG_TYPE_INTERNAL_SOURCE,

}emuconfig_type_t;

//every single different 'default' configuration in the emulator configuration list
typedef enum emulator_internal_source_type
{
	INPUT_CONFIG_INTERNAL_INVAL,
	INPUT_CONFIG_INTERNAL_BASIC_CONSOLE,
}emu_internal_src_t;


typedef struct wcpu_simulator_input_config
{
	emuconfig_type_t config_type;
	emu_internal_src_t internal_type;
	sourcefile_t *sourcefile;

	WS_sim_input_t **settings;
	int settings_index;
	int settings_size;
}WS_input_config_t;

extern char *internal_source_paths[];

//used to test a device name against a compared device, uses emuconfig_dev_type_name
static bool test_device_name(WS_dev_desc_t *desc, const char *tocmp);
static WS_input_config_t *create_config_ptr(void);
static bool config_setting(WS_input_config_t *ic);


WS_input_config_t *WS_create_input_config_internal(const char *path);

WS_input_config_t *WS_create_input_config_external(emu_internal_src_t src);

#endif