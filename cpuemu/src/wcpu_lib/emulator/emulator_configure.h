#ifndef __EMULATOR_CONFIGURE_HEADER__
#define __EMULATOR_CONFIGURE_HEADER__

#include "loader.h"
#include "device_type_ptr.h"
#include "token.h"

#include <stdbool.h>

#define EMULATOR_CONFIGURATION_MAXIMUM_DEVICES_SETUP 255

//used for the 'main' configuration type of the emulator config, either default(internal) or set(external)
typedef enum emuconfig_type
{
	EMUCONFIG_TYPE_INVAL,

	EMUCONFIG_TYPE_EXTERNAL_SOURCE,
	EMUCONFIG_TYPE_INTERNAL_SOURCE,

}emuconfig_type_t;

//every single different 'default' configuration in the emulator configuration list
typedef enum emulator_internal_source_type
{
	EMUCONFIG_INTERNAL_INVAL,
	EMUCONFIG_INTERNAL_BASIC_CONSOLE,
}emu_internal_src_t;




//all the keywords that define the 'device'
//should be the same size as the device_type_t



typedef struct emulator_config_device_settings
{
	bool used;
	device_type_t type;
	toklex_t *toklex;


}emuconfig_dev_settings_t;


//the argument parser for each device
typedef bool (*emuconfig_device_argparser_t)(emuconfig_dev_settings_t *settings);

//the device type
typedef struct emuconfig_device_type
{
	emuconfig_device_argparser_t argparser_fn;
	char *keyword;
} emuconfig_dev_type_t;

extern emuconfig_dev_type_t emuconfig_dev_type_table[DEVICES_TYPE_COUNT];








typedef struct emulator_config
{
	emuconfig_type_t config_type;
	emu_internal_src_t internal_type;
	sourcefile_t *sourcefile;

	emuconfig_dev_settings_t settings[EMULATOR_CONFIGURATION_MAXIMUM_DEVICES_SETUP];
	int settings_index;

}emuconfig_t;

extern char *internal_source_paths[];

//used to test a device name against a compared device, uses emuconfig_dev_type_name
static bool test_device_name(device_type_t device_type, const char *tocmp);
static emuconfig_t *create_config_ptr(void);
static bool config_setting(emuconfig_t *ec);

emuconfig_t *create_emuconfig_external(char *path);

emuconfig_t *create_emuconfig_internal(emu_internal_src_t src);

#endif