


#include "manager.h"
#include "core.h"
#include "coreutils.h"
#include "cli.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "code_decoder.h"
#include "hydra.h"

void fill_binary(uint64_t *bin, size_t length)
{

	for(size_t i = 0; i < length; ++i)
	{

		memory_write(components.mem, i, bin[i] );
	}
}

size_t file_len(FILE *fp)
{
	if(!fp)
		return 0;

	size_t current = ftell(fp);

	fseek(fp, 0, SEEK_END);
	size_t address = ftell(fp);

	fseek(fp, current, SEEK_SET);
	return address;
}


void load_file(const char *file_name)
{
	FILE *fp = fopen(file_name, "rb");
	if(fp == NULL)
	{
		printf("file: %s not openable\n", file_name);
		exit(1);

	}
	size_t len = file_len(fp);

	char *bytes = (char *)calloc(len, sizeof(char));

	fread(bytes, 8, len / 8, fp);
	uint64_t *bin = (uint64_t *)bytes;

	fill_binary(bin, len / sizeof(uint64_t));

}


globalstate_t globalstate =
{
	.argc = 0,
	.argv = 0,
	.ctx = {0},
	.has_debuger = false,
	.breakpoints = {-1},
	.running = false,
	.runfor = 0
};

void init(int argc, char **argv)
{

	globalstate.argc = argc;
	globalstate.argv = argv;
	create_cli_context(&globalstate.ctx);


	init_components();
	create_cpu();

	if(argc == 2)
	{
		load_file(argv[1]);
		startup_cpu();
	}
	update();
}

cmd_t *input(void)
{
	return pull_line(&globalstate.ctx);
}

int on_breakpoint(void)
{
	int pc = get_pc();
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
	if(!globalstate.running)
	{
		return;
	}
	if(globalstate.runfor < 0)
	{
		globalstate.running = 0;
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
		globalstate.running = false;
		return;
	}
	for(int i = 0; i < 5; ++i)
	{
		step_cpu();

	}

	if(components.cpu->stop == true)
	{
		globalstate.running = false;
		globalstate.runfor = 0;
		components.cpu->stop = false;
	}

}

void print_cmd(cmd_t *cmd)
{




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
			while(globalstate.running)
			{
				step_handler();

			}

		}

	}




}
