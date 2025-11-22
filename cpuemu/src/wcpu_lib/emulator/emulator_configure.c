#include "loader.h"
#include "emulator_configure.h"
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

char *internal_source_paths[] =
{
	[EMUCONFIG_INTERNAL_INVAL] = "inval",
	[EMUCONFIG_INTERNAL_BASIC_CONSOLE] = "configfiles/basic_config.txt",

};


static bool argparser_wcpu(emuconfig_dev_settings_t *settings)
{
	settings->type = DEVICE_WCPU;
	return true;
}
static bool argparser_ram(emuconfig_dev_settings_t *settings)
{
	settings->type = DEVICE_RAM;
	return true;

}
static bool argparser_fakeio(emuconfig_dev_settings_t *settings)
{
	settings->type = DEVICE_FAKEIO;
	return true;

}

//device type keyword list
emuconfig_dev_type_t emuconfig_dev_type_table[DEVICES_TYPE_COUNT] =
{
	[DEVICE_INVAL] = {.argparser_fn = NULL, .keyword = "INVAL"},
	[DEVICE_RAM] = {.argparser_fn = argparser_ram, .keyword = "RAM"},
	[DEVICE_FAKEIO] = {.argparser_fn = argparser_fakeio, .keyword = "FAKEIO"},
	[DEVICE_WCPU] = {.argparser_fn = argparser_wcpu, .keyword = "WCPU"},
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
	emuconfig_dev_settings_t setting = {0};
	char *line = readline_sourcefile(ec->sourcefile);

	setting.toklex = lex_string(line);
	free(line);

	reset_toklex(setting.toklex);
	bool expects = expect_toklex(setting.toklex, TOK_WORD);
	if(expects == false)
	{
		free_toklex(setting.toklex);
		return false;
	}


	bool valid_first_tok = false;
	//print_toklex(setting.toklex);

	tok_t *first = get_toklex(setting.toklex, 0);
	if(first == NULL)
	{
		perror("first keyword in configuration failed");
		exit(EXIT_FAILURE);
	}
	char *first_str = first->token;
			printf("%s\n", first_str);

	for(int i = 0; i < DEVICES_TYPE_COUNT; ++i)
	{
		if(test_device_name((device_type_t) i, first_str))
		{

			emuconfig_device_argparser_t argparser = emuconfig_dev_type_table[i].argparser_fn;
			if(argparser == NULL)
			{
				perror("argument parser for this type is null");
				return false;

			}
			bool success = argparser(&setting);
			if(success == false)
			{
				//TODO the argument is not valid and we must stop it
				perror("argument parse failed for a line");
				return false;

			}
			valid_first_tok = true;
		}
	}


	if(valid_first_tok)
	{
		setting.used = true;
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
		if(config_setting(ec) == false)
			break;
	}


	close_sourcefile(ec->sourcefile);

	return true;
}


emuconfig_t *create_emuconfig_external(char *path)
{
	emuconfig_t *config = create_config_ptr();
	config->config_type = EMUCONFIG_TYPE_EXTERNAL_SOURCE;


	sourcefile_t *sf = create_sourcefile(path);
	config->sourcefile = sf;


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


	config->config_type = EMUCONFIG_TYPE_INTERNAL_SOURCE;
	sourcefile_t *sf = create_sourcefile(internal_source_paths[src]);
	config->sourcefile = sf;
	if(get_canopen_sourcefile(sf) == false)
	{
		perror("cannot open internal source file");
		exit(EXIT_FAILURE);

	}



	parse_emuconfig_setting(config);


	return config;
}