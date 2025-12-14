#ifndef __EMULATOR_DEVICE_ARGPARSER_HEADER__
#define __EMULATOR_DEVICE_ARGPARSER_HEADER__


#include <stdbool.h>
#include "device_command.h"
#include "device_list.h"
#include "device_description.h"
#include "token.h"



//the argument parser for each device


typedef struct wcpu_simulator_input
{
	//the lexer
	toklex_t *toklex;
	//the command
	device_command_t *command;
}WS_sim_input_t;
//the device type

typedef bool (*WS_sim_input_parser_fn_t)(WS_sim_input_t *imp);



//all the keywords that define the 'device'

//should be the same size as the device_type_t

/*

extern emuconfig_dev_type_t emuconfig_dev_type_table[DEVICES_LIST_TYPE_COUNT];

*/

//the argument parser for each device type
//solves the 'inital' configure lex'






#endif
