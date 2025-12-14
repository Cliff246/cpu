#include "loader.h"
#include "emulator_configure.h"
#include "device_list.h"

#include "emulator_device_argparser.h"

#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

/*

char *internal_source_paths[] =
{
	[EMUCONFIG_INTERNAL_INVAL] = "inval",
	[EMUCONFIG_INTERNAL_BASIC_CONSOLE] = "configfiles/basic_config.txt",

};



static bool test_device_name(device_type_t device_type, const char *tocmp)
{
	//cannot test a device that is 'invalid'
	if(device_type == DEVICE_INVAL)
	{
		return false;
	}

	if(strcmp(tocmp, emuconfig_dev_type_table[device_type].keyword) == 0)
	{
		return true;
	}
	return false;
}




static emuconfig_t *create_config_ptr(void)
{
	emuconfig_t *ptr = calloc(1, sizeof(emuconfig_t));
	if(ptr == NULL)
	{
		perror("emu config failed to calloc");
		exit(EXIT_FAILURE);
	}
	ptr->config_type = EMUCONFIG_TYPE_INVAL;
	ptr->internal_type = EMUCONFIG_INTERNAL_INVAL;
	return ptr;
}




static bool config_setting(emuconfig_t *ec)
{
	//create device settings
	emuconfig_dev_settings_t *setting = calloc(1, sizeof(emuconfig_dev_settings_t));
	assert(setting != NULL && "malloc cannot fail");
	//read a line in the source file
	char *line = readline_sourcefile(ec->sourcefile);
	//lex the string
	setting->toklex = lex_string(line);
	//toklex copies we dont need this
	free(line);
	//reset so we can interate again
	reset_toklex(setting->toklex);
	//expect a new
	bool expects = expect_toklex(setting->toklex, TOK_WORD);
	if(expects == false)
	{
		free_toklex(setting->toklex);
		return false;
	}


	bool valid_first_tok = false;
	//print_toklex(setting->toklex);

	tok_t *first = get_toklex(setting->toklex, 0);
	if(first == NULL)
	{
		perror("first keyword in configuration failed");
		exit(EXIT_FAILURE);
	}
	char *first_str = first->token;

	for(int i = 0; i < DEVICES_LIST_TYPE_COUNT; ++i)
	{
		if(test_device_name((device_type_t) i, first_str))
		{

			emuconfig_dev_type_t *argparser = &emuconfig_dev_type_table[i];
			if(argparser == NULL)
			{
				perror("argument parser for this type is null");
				return false;

			}
			bool success = emulator_device_argparser_head(setting, argparser);
			if(success == false)
			{
				//TODO the argument is not valid and we must stop it
				perror("argument parse failed for a line");
				assert(0);
				return false;

			}
			valid_first_tok = true;
		}
	}


	if(valid_first_tok)
	{
		//printf("%d %d\n", ec->settings_size, ec->settings_index);
		assert(ec->settings_size > ec->settings_index && "cannot over fill the amount of settings in a emuconfig");
		ec->settings[ec->settings_index++] = setting;
		return true;
	}
	else
	{
		perror("no valid first token found");
		return false;
	}

}


static bool parse_emuconfig_setting(emuconfig_t *ec)
{
	if(ec == NULL)
		return false;

	if(open_sourcefile(ec->sourcefile) == false)
	{
		perror("open source file failed at parse\n");
		exit(1);
	}

	while(!get_isend_sourcefile(ec->sourcefile))
	{
		assert(config_setting(ec) == true && "cannot configure a invalid setting");
	}


	close_sourcefile(ec->sourcefile);

	return true;
}


emuconfig_t *create_emuconfig_external(char *path)
{
	emuconfig_t *config = create_config_ptr();
	assert(config);
	config->config_type = EMUCONFIG_TYPE_EXTERNAL_SOURCE;


	sourcefile_t *sf = create_sourcefile(path);
	assert(sf);
	config->sourcefile = sf;


	config->settings_size = 255;
	config->settings = calloc(config->settings_size, sizeof(emuconfig_dev_settings_t*));
	assert(config->settings);

	if(get_canopen_sourcefile(sf) == false)
	{
		perror("cannot open external source file");
		exit(EXIT_FAILURE);

	}
	return config;

}

emuconfig_t *create_emuconfig_internal(emu_internal_src_t src)
{
	if(src == EMUCONFIG_INTERNAL_INVAL)
	{
		perror("cannot create internal emulator source with EMUCONFIG_INTERNAL_INVAL");
		exit(EXIT_FAILURE);
	}
	emuconfig_t *config = create_config_ptr();
	assert(config);

	config->config_type = EMUCONFIG_TYPE_INTERNAL_SOURCE;
	sourcefile_t *sf = create_sourcefile(internal_source_paths[src]);
	assert(sf);

	config->sourcefile = sf;
	if(get_canopen_sourcefile(sf) == false)
	{
		perror("cannot open internal source file");
		exit(EXIT_FAILURE);

	}


	config->settings_size = 255;
	config->settings = calloc(config->settings_size, sizeof(emuconfig_dev_settings_t*));
	assert(config->settings);

	assert(parse_emuconfig_setting(config));


	return config;
}

*/