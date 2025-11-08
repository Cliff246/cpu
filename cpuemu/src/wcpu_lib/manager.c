


#include "manager.h"
#include "core.h"
#include "coreutils.h"
#include "cli.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "code_decoder.h"

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
}

void step_handler(void)
{
	if(!globalstate.running)
	{
		return;
	}
	if(globalstate.runfor <= 0)
	{
		globalstate.running = 0;
		return;
	}
	else
	{
		globalstate.runfor--;
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



}

void print_cmd(cmd_t *cmd)
{


	tok_t *tok1 = get_tok_cmd(cmd, 1);
	char *arg1 = tok1->token;
	if(!strcmp(arg1, "regs"))
	{
		print_regs();
	}
	else if(!strcmp(arg1, "reg"))
	{
		tok_t *tok2 = get_tok_cmd(cmd, 2);
		if(tok2)
			return;
		if(tok2->type == TOK_INT)
		{
			int reg = atoi(tok2->token);
			printf("r:%d = %d", reg, get_reg(reg));
			return;
		}
	}
	else if(!strcmp(arg1, "mem"))
	{

		tok_t *tok2 = get_tok_cmd(cmd, 2);
		tok_t *tok3 = get_tok_cmd(cmd, 3);
		if(!tok2 || !tok3)
		{
			memory_print(components.mem, 0, 50);

			return;

		}
		if(tok2->type == TOK_INT && tok3->type == TOK_INT)
		{
			int start = atoi(tok2->token);
			int stop = atoi(tok3->token);
			memory_print(components.mem, start, stop);

		}
		else
		{
			memory_print(components.mem, 0, 50);

		}


	}
	else if(!strcmp(arg1, "inst"))
	{
		inst_t inst = decode_inst(components.cpu->curins);
		//print_inst(&inst);
		char *itext = convert_inst_to_text(&inst);
		printf("%s\n", itext);
		free(itext);
	}
	else
	{
		printf("invalid print statement\n");
	}


}


void step_cmd(cmd_t *cmd)
{
	step_cpu();
	step_cpu();
	step_cpu();
	step_cpu();
	step_cpu();
}

void break_cmd(cmd_t *cmd)
{
	char *arg1 = get_elem_line(&cmd->line, 1);

}

void load_cmd(cmd_t *cmd)
{
	tok_t *tok1 = get_tok_cmd(cmd, 1);
	if(tok1->type == TOK_STRING)
	{
		load_file(tok1->token);

	}

}

void run_cmd(cmd_t *cmd)
{
	char *arg1 = get_elem_line(&cmd->line, 1);
	if(arg1 == NULL)
	{
		globalstate.runfor = 100;
		globalstate.running = true;
	}
	else
	{
		globalstate.runfor = atoi(arg1);
		globalstate.running = true;

	}

}


bool basic_input_manager(cmd_t *cmd)
{
	char *tok1 = get_elem_line(&cmd->line, 0);
	if(!strcmp(tok1, "print"))
	{
		print_cmd(cmd);
		return false;
	}
	else if(!strcmp(tok1,  "step"))
	{
		step_cmd(cmd);
		return false;
	}
	else if(!strcmp(tok1, "break"))
	{
		break_cmd(cmd);
		return false;
	}
	else if(!strcmp(tok1, "load"))
	{
		load_cmd(cmd);
		return false;

	}
	else if(!strcmp(tok1, "run"))
	{
		run_cmd(cmd);
		return true;
	}
	else if(!strcmp(tok1, "init"))
	{
		startup_cpu();
		return false;
	}
	else
	{
		printf("invalid command\n");
		return false;
	}


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
