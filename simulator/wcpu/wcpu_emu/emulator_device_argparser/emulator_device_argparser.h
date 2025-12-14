#ifndef __EMULATOR_DEVICE_ARGPARSER_HEADER__
#define __EMULATOR_DEVICE_ARGPARSER_HEADER__


#include <stdbool.h>
#include "device_command.h"
#include "device_list.h"
#include "token.h"



//the argument parser for each device


typedef struct emulator_config_device_settings
{
	//the lexer
	toklex_t *toklex;
	//the command
	device_command_t *command;
}emuconfig_dev_settings_t;
//the device type

typedef bool (*emuconfig_device_argparser_t)(emuconfig_dev_settings_t *settings);



//all the keywords that define the 'device'

//should be the same size as the device_type_t
typedef struct emuconfig_device_type
{
	emuconfig_device_argparser_t argparser_fn;
	device_type_t type;
	char *keyword;
} emuconfig_dev_type_t;

/*

extern emuconfig_dev_type_t emuconfig_dev_type_table[DEVICES_LIST_TYPE_COUNT];

*/

//the argument parser for each device type
//solves the 'inital' configure lex'


bool emulator_device_argparser_head(emuconfig_dev_settings_t *settings, emuconfig_dev_type_t *type);




#endif
