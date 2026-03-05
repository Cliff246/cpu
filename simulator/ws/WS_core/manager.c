

#include "loader.h"
#include "manager.h"
//#include "core.h"
//#include "coreutils.h"
#include "MANFST_manifest.h"
#include "TAG_bool.h"
#include "TAG_list.h"
#include "TAG_tag.h"
#include "TAG_int.h"
#include "cli.h"
#include "commons.h"
#include "export.h"

#include "SYNTAX_parser.h"

#include "SIM_simulator.h"
#include "hashmap.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>


globalstate_t globalstate =
{
	.args = {.argc = 0, .argv = 0},
	.breakpoints = {-1},
	.runfor = 0,
	.loaded = {.dynamic_libs = NULL, .count = 0,}

};

#define GLBST globalstate


bool get_flag(gsflag_t flag)
{
	return GLBST.flags[flag];
}

void set_flag(gsflag_t flag)
{
	GLBST.flags[flag] = true;
}

void clr_flag(gsflag_t flag)
{
	GLBST.flags[flag] = false;
}

#define IS_RUNNING get_flag(FLAG_RUNNING)


typedef void (*handle_argstate)(char *, bool);
#define HANDLE_STATE(name) void handle_ ## name ## _state (char *keyword, bool reset)

struct argstate
{
	char *string;
	bool set;
	int follows;
	int current;
	handle_argstate handle;
};


enum arg_state_class
{
	__debug_state,
	__export_state,
	__log_state,
	__source_state,
	__ignore_state,
	__test_state,
	__set_state,
	//do not remove the end
	__end__,
};


HANDLE_STATE(debug)
{
	logger_set = true;
}

HANDLE_STATE(source)
{
	static int count = 0;
	//printf("source %s\n", keyword);
	if(get_flag(FLAG_HAS_SOURCE))
	{
		printf("too many exports %s\n", keyword);
		exit(1);
	}

	if(reset)
		count = 0;
	else
		count++;


	if(count == 1)
	{

		globalstate.source_path = strdup(keyword);
		set_flag(FLAG_HAS_SOURCE);
	}
}

HANDLE_STATE(export)
{
	static int count = 0;

	if(get_flag(FLAG_EXPORT))
	{
		printf("too many exports %s\n", keyword);
		exit(1);
	}

	if(reset)
		count = 0;
	else
		count++;


	if(count == 1)
	{

		globalstate.export_path = strdup(keyword);
		set_flag(FLAG_EXPORT);
	}
}

HANDLE_STATE(ignore)
{
	set_flag(FLAG_IGNORE_BREAK);
}

HANDLE_STATE(log)
{

	static bool log_state = 0;

	if(reset)
	{

	}
}

HANDLE_STATE(test)
{
	static bool testing_enabled = false;
	if(testing_enabled == true)
	{
		return;
	}

	set_flag(FLAG_TESTING);

}

HANDLE_STATE(set)
{

}


#define FILL_ARGSTATE(keyword, holdfor, function) {.string = keyword, .set = false, .follows = holdfor,  .handle = function}

struct argstate states[] =
{
	[__debug_state] = FILL_ARGSTATE("d", 0, handle_debug_state),
	[__export_state] = FILL_ARGSTATE("e", 1, handle_export_state ),
	[__log_state] = FILL_ARGSTATE("l", 1, handle_export_state),
	[__source_state] = FILL_ARGSTATE("s", 1, handle_source_state),
	[__ignore_state] = FILL_ARGSTATE("i", 1, handle_ignore_state),
	[__test_state] = FILL_ARGSTATE("test", 0, handle_test_state),
	[__set_state] = FILL_ARGSTATE("=", 1, handle_set_state),

	//do not move end
	[__end__] = {.follows = 0, .handle = NULL, .set = false, .string = "INVALID STRING DONT USE LOL"},
};

#define STATES_LEN (ARYSIZE(states) - 1)


void parse_args(void)
{
	static bool hasparsed = false;

	if(hasparsed == true)
		return;
	hasparsed = true;
	global_arguments_t ga = GLBST.args;

	if(ga.argc == 1)
	{
		printf("not enough args\n");
		exit(1);
	}
	if(ga.argc >= MAX_EMUARGUMENTS)
	{
		printf("too many arguments\n");
		exit(1);
	}

	for(int i = 0; i < ga.argc && i < MAX_EMUARGUMENTS; ++i)
	{
		char *tmp_arg = ga.argv[i];
		//check iterator
		for(int ci = 0; ci < STATES_LEN; ++ci)
		{
			if(states[ci].set == true)
			{
				//printf("is set %d\n", ci);
				struct argstate *tmp_state = &states[ci];
				tmp_state->current --;
				if(tmp_state->current <= 0)
				{
					tmp_state->set = false;
				}
				tmp_state->handle(tmp_arg, false);
			}
		}


		if(tmp_arg[0] == '-' )
		{
			char *next = tmp_arg + 1;
			//state iterator
			bool found_one = false;
			for(int si = 0; si < STATES_LEN; ++si)
			{
				if(!strcmp(next, states[si].string))
				{
					//printf("found one %s\n", next);
 					states[si].set = true;
					states[si].current = states[si].follows;
					states[si].handle(next, true);
					found_one = true;
				}
			}
			if(found_one == false)
			{
				printf("invalid argument %s \n", tmp_arg);
				exit(1);
			}

		}

	}

}


void init(int argc, char **argv)
{

	sourcefile_t *sf1 = create_sourcefile("configfiles/basic_new_config.txt");
	toklex_t *lex1= lex_string(read_all_sourcefile(sf1));
//	print_toklex(lex);
	SYNTAX_ptree_t *tree1 = SYNTAX_ptree_create(lex1);
	SYNTAX_ptree_parse(tree1);


	//sourcefile_t *sf2 = create_sourcefile("configfiles/config_3_0.txt");
	//toklex_t *lex2= lex_string(read_all_sourcefile(sf2));
//	print_toklex(lex);
	//SYNTAX_ptree_t *tree2 = SYNTAX_ptree_create(lex2);
	//SYNTAX_ptree_parse(tree2);


	//SYNTAX_pnode_print(tree->head, 0);
	//SYNTAX_ptree_free(tree);
	//SYNTAX_pnode_print(tree->settings, 0);
	MANFST_manifest_t *manifest1 =  MANFST_init_manifest(tree1);
	//MANFST_manifest_t *manifest2 =  MANFST_init_manifest(tree2);

	SIM_simulator_t *sim = SIM_init_simulator();

	SIM_load_manifest_simulator(sim, manifest1);
	MANFST_free_manifest(manifest1);
	//SIM_load_manifest_simulator(sim, manifest2);
	//MANFST_free_manifest(manifest2);

	SIM_assign_scene_to_stage_simulator(sim);
	SIM_start_simulator(sim);



	parse_args();


	if(get_flag(FLAG_HAS_SOURCE))
	{
		//load_file(GLBST.source_path);
	}

	if(get_flag(FLAG_TESTING))
	{

	}
	else
	{
		//update();

	}

}
