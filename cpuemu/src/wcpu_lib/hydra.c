#include "hydra.h"
#include "core.h"
#include "manager.h"
#include "coreutils.h"
#include "code_decoder.h"

#include <stdbool.h>

#include <stdio.h>
#include <string.h>


hydra_entry_t hydra_list[] =
{
	{"print",  print_head},
	{"step", step_head},
	{"break", break_head},
	{"run", run_head},
	{"load", load_head},
	{NULL, NULL}
};



void hydra(cmd_t *cmd)
{
	tok_t *first_keyword = next_tok_cmd(cmd);
	if(first_keyword == NULL)
	{
		printf("must enter line\n");
	}
	else
	{
		bool ran = false;
		hydra_entry_t *entry = hydra_list;
		//linear search yes but so be it
		//well mostly linear
		char *keyword = first_keyword->token;
		while(entry->keyword != NULL)
		{
			if(!strcmp(keyword, entry->keyword))
			{
				entry->head(cmd);
				ran = true;
				break;
			}
			else
			{
				entry++;
			}
		}
		if(ran == false)
		{
			printf("cmd not recognized\n");
		}
	}


}

void step_head(cmd_t *cmd)
{
	step_cpu();
	step_cpu();
	step_cpu();
	step_cpu();
	step_cpu();
}

void break_head(cmd_t *cmd)
{
	char *arg1 = get_elem_line(&cmd->line, 1);
}

void load_head(cmd_t *cmd)
{
	tok_t *tok1 = next_tok_cmd(cmd);
	if(tok1->type == TOK_STRING)
	{
		load_file(tok1->token);

	}
}

void print_head(cmd_t *cmd)
{
	tok_t *tok1 = next_tok_cmd(cmd);
	char *arg1 = tok1->token;
	if(!strcmp(arg1, "regs"))
	{
		print_regs();
	}
	else if(!strcmp(arg1, "reg"))
	{
		tok_t *tok2 = next_tok_cmd(cmd);
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

		tok_t *tok2 = next_tok_cmd(cmd);
		tok_t *tok3 = next_tok_cmd(cmd);
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

void run_head(cmd_t *cmd)
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