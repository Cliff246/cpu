


#include "manager.h"
//#include "core.h"
//#include "coreutils.h"
#include "cli.h"
#include "commons.h"
#include "code_decoder.h"
#include "hydra.h"
#include "export.h"
#include "simulator.h"
#include "ws_sim_configure.h"
#include "device_commons.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

//static cpu_t *global_cpu(void)
//{
	//return components.cpu;
//}

static size_t file_len(FILE *fp)
{
	assert(fp);

	size_t current = ftell(fp);

	fseek(fp, 0, SEEK_END);
	size_t address = ftell(fp);

	fseek(fp, current, SEEK_SET);
	return address;
}
/*
//returns true on change to size and false on no updated size
static bool load_file(vima_t *vm, const char *file_name)
{
	FILE *fp = fopen(file_name, "rb");
	if(fp == NULL)
	{
		printf("file: %s not openable\n", file_name);
		exit(1);

	}
	size_t len = file_len(fp);
	char *bytes = (char *)calloc(len, sizeof(char));
	assert(bytes);
	fread(bytes, 8, len / 8, fp);

	bool changed = false;


	uint64_t *bin = (uint64_t *)bytes;

	vm_setmemory(vm,bin, len / sizeof(uint64_t));
	free(bytes);
	fclose(fp);
	return changed;
}
*/

globalstate_t globalstate =
{
	.args = {.argc = 0, .argv = 0},
	.breakpoints = {-1},
	.runfor = 0,
	.loaded = {.modules = NULL, .count = 0,}

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


static enum arg_state_class
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

static void load_module(const char *module)
{


	WS_module_t **temp = (WS_module_t **)realloc(globalstate.loaded.modules, (globalstate.loaded.count + 1) * sizeof(WS_module_t *));
	assert(temp);
	globalstate.loaded.modules = temp;

	globalstate.loaded.modules[globalstate.loaded.count++] = WS_module_create(module);



}

void init(int argc, char **argv)
{
	/*
	emuconfig_t *config = create_emuconfig_internal(EMUCONFIG_INTERNAL_BASIC_CONSOLE);


	emulator_t *emu = 	emulator_generate(config);

	vima_t *vima = vm_init(1000);

	load_file(vima, "a.bin");
	vm_cpu_set_inital_cd(vima);
	for(int i = 0; i < 1001; ++i)
	{
		vm_step(vima);
	}
	vm_print_mem(vima, 0, 25);
	vm_cpu_print_regs(vima);
	exit(1);
	*/
	logger_set = false;
	globalstate.args.argc = argc;
	globalstate.args.argv = argv;
	//after seting args

	WS_config_file_t *file = WS_create_config_file("configfiles/basic_config.txt");

	WS_simulator_t *sim = WS_simulator_init();
	WS_simulator_load_config(sim, file);
	WS_free_config_file(file);


	for(int i = 0; i < 5000; ++i)
	{
		WS_simulator_update(sim);
		//printf("%d\n", i);
	}

	WS_simulator_print_all_devices(sim);

	parse_args();
	//create_cli_context(&globalstate.ctx);











	if(get_flag(FLAG_HAS_SOURCE))
	{
		//load_file(GLBST.source_path);
	}

	if(get_flag(FLAG_TESTING))
	{
		//testing();
		//basic_export();

	}
	else
	{
		//update();

	}

}

//cmd_t *input(void)
//{
//	return pull_line(&globalstate.ctx);
//}

/*
int on_breakpoint(void)
{
	//int pc = get_pc();
	for(int i = 0; i < MAX_BREAKPOINTS; ++i)
	{
		int temp = globalstate.breakpoints[i];
		if(temp < 0)
		{
			return -1;
		}
		else if(temp == pc)
		{
			return i;
		}
	}
	return -1;
}

void step_handler(void)
{

	if(globalstate.runfor < 0)
	{
		clr_flag(FLAG_RUNNING);
		return;
	}
	else
	{
		globalstate.runfor--;
		//printf("%d\n",globalstate.runfor);
	}
	int brk = on_breakpoint();
	if(brk != -1)
	{
		clr_flag(FLAG_RUNNING);
		return;
	}
	for(int i = 0; i < 5; ++i)
	{
		//step_cpu();

	}
	//cpu_t *cpu = global_cpu();
	//if(cpu->stop == true && get_flag(FLAG_IGNORE_BREAK) == false)
	{
		clr_flag(FLAG_RUNNING);

		globalstate.runfor = 0;
		//cpu->stop = false;
	}

}

void print_cmd(cmd_t *cmd)
{




}

void basic_export(void)
{
	if(!get_flag(FLAG_EXPORT))
		return;

	export_bundle_t *bnd = create_export_bundle(GLBST.export_path);

	for(int i = 0; i < 64; ++i)
	{
		export_packet_t regexp = create_packet_register(i, 0, 0, get_reg(i), EXPORT_MODE_INT);
		add_packet_to_bundle(bnd, regexp);
	}

	for(int im = 0; im < 100; ++im)
	{
		export_packet_t memexp = create_packet_memory(load(im), im);
		add_packet_to_bundle(bnd, memexp);

	}

	export_dump(bnd);
}



void step_cmd(cmd_t *cmd)
{

}

void break_cmd(cmd_t *cmd)
{


}

void load_cmd(cmd_t *cmd)
{


}

void run_cmd(cmd_t *cmd)
{


}


bool basic_input_manager(cmd_t *cmd)
{
	bool run = hydra(cmd);
	return run;
}




void update(void)
{
	bool running = true;


	while(running)
	{
		cmd_t *inp = input();
		bool skip = basic_input_manager(inp);
		if(skip)
		{
			while(IS_RUNNING)
			{
				step_handler();

			}

		}

	}




}


#define TEST_CYCLES 1000
void testing(void)
{
	//TODO make this actually not like static
	for(int i = 0; i < TEST_CYCLES; ++i)
	{
		for(int i = 0; i < 5; ++i)
		{
			//if(global_cpu()->program_over)
			{
				return;
			}
			//step_cpu();

		}

	}
}
*/