#include "emulator_configure.h"
#include "emulator_device_argparser.h"
#include "device_command.h"
#include "device_list.h"


#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

static bool argparser_wcpu(emuconfig_dev_settings_t *settings)
{
	return true;
}
static bool argparser_ram(emuconfig_dev_settings_t *settings)
{


	dev_ram_config_setting_t *ram = settings->command->setting.ram;
	toklex_t *tl = settings->toklex;

	bool success = true;

	while(true)
	{
		tok_t *tok = peek_toklex(tl);
		if(tok == NULL)
			break;
		else if(tok->type == TOK_END)
			break;
		else if(tok->type == TOK_STRING)
		{
			//fix this as well... also really dangerous
			ram->use_filename = true;
			ram->filename = strdup(tok->token);
			printf("using filename %s\n",ram->filename);
		}
		else if(tok->type == TOK_INT)
		{
			ram->use_size = true;
			//TODO fix this
			//this is very dangerous
			ram->size = atoi(tok->token);
		}
		else
		{
			success = false;
			break;
		}
	}



	return success;

}
static bool argparser_fakeio(emuconfig_dev_settings_t *settings)
{
	return true;

}


//device type keyword list
//this is used in emulator_config
emuconfig_dev_type_t emuconfig_dev_type_table[DEVICES_LIST_TYPE_COUNT] =
{
	[DEVICE_INVAL] = {
		.argparser_fn = NULL,
		.keyword = "INVAL",
		.type = DEVICE_INVAL,
		},

	[DEVICE_RAM] = {
		.argparser_fn = argparser_ram,
		.keyword = "RAM",
		.type = DEVICE_RAM,
		},

	[DEVICE_FAKEIO] = {
		.argparser_fn = argparser_fakeio,
		.keyword = "FAKEIO",
		.type = DEVICE_FAKEIO,
		},

	[DEVICE_WCPU] = {
			.argparser_fn = argparser_wcpu,
		 	.keyword = "WCPU",
			.type = DEVICE_WCPU,
		},
};

bool emulator_device_argparser_head(emuconfig_dev_settings_t *settings, emuconfig_dev_type_t *type)
{
	assert(type != NULL);
	assert(settings != NULL);


	assert(type->argparser_fn != NULL);
	assert(type->type != DEVICE_INVAL);


	//pre allocate the device command size
	settings->command = device_command_generate(type->type);
	assert(settings->command != NULL);
	settings->command->type = type->type;
	printf("command type %d\n", settings->command->type);
	bool result = type->argparser_fn(settings);
	return result;
}
