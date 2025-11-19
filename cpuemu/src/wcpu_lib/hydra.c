#include "hydra.h"
//#include "core.h"
#include "manager.h"
//#include "coreutils.h"
#include "code_decoder.h"
//#include "disassembler.h"
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
	{"exit", exit_head},
	{NULL, NULL}
};



bool hydra(cmd_t *cmd)
{
	tok_t *first_keyword = next_tok_cmd(cmd);
	if(first_keyword == NULL)
	{
		printf("must enter line\n");
	}
	else
	{
		bool ran = false, result = false;
		hydra_entry_t *entry = hydra_list;
		//linear search yes but so be it
		//well mostly linear
		char *keyword = first_keyword->token;
		while(entry->keyword != NULL)
		{
			if(!strcmp(keyword, entry->keyword))
			{
				result = entry->head(cmd);
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
			return false;
		}
		return result;
	}
	return false;

}

bool step_head(cmd_t *cmd)
{
	step_cpu();
	step_cpu();
	step_cpu();
	step_cpu();
	step_cpu();
	tok_t *tok1 = next_tok_cmd(cmd);
	return false;
}

bool exit_head(cmd_t *cmd)
{
	basic_export();
	exit(0);
}

bool break_head(cmd_t *cmd)
{

	tok_t *tok1 = next_tok_cmd(cmd);
	return false;
}

bool load_head(cmd_t *cmd)
{
	tok_t *tok1 = next_tok_cmd(cmd);
	if(tok1->type == TOK_STRING)
	{
		load_file(tok1->token);

	}
	return false;
}

bool print_head(cmd_t *cmd)
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
			return false;
		if(tok2->type == TOK_INT)
		{
			int reg = atoi(tok2->token);
			printf("r:%d = %lld", reg, get_reg(reg));
			return false;
		}
	}
	else if(!strcmp(arg1, "sp"))
	{
		printf("%d\n", get_sp());
	}
	else if(!strcmp(arg1, "sfp"))
	{
		printf("%d\n", get_sfp());
	}
	else if(!strcmp(arg1, "mem"))
	{

		tok_t *tok2 = next_tok_cmd(cmd);
		tok_t *tok3 = next_tok_cmd(cmd);
		if(!tok2 || !tok3)
		{
			//memory_print(components.mem, 0, 50);

			return false;

		}
		if(tok2->type == TOK_INT && tok3->type == TOK_INT)
		{
			int start = atoi(tok2->token);
			int stop = atoi(tok3->token);
			//memory_print(components.mem, start, stop);

		}
		else
		{
			//memory_print(components.mem, 0, 50);

		}


	}
	else if(!strcmp(arg1, "inst"))
	{
		//print_inst(&inst);

		//operation_t *op = &components.cpu->op;
		//printf("decoded %d\n", op->inst.decoded);
		//char *itext = convert_operation_to_text(op);
		//printf("%s\n", itext);
		//free(itext);
	}
	else if(!strcmp(arg1, "block"))
	{
/*
		//code_block_t *block = create_block_from_current_description(components.cpu, 0, 100);

		for(int i =  0; i < block->op_count ; ++i)
		{
			char *c = convert_operation_to_text(&block->ops[i]);
			if(get_pc() == i)
			{
				printf("%3d: \t%s\n",i, c);
			}
			else
			{
				printf("%3d: %s\n", i, c);

			}
			free(c);
		}
		free(block->ops);
		free(block);
*/
	}
	else
	{
		printf("invalid print statement\n");
	}

	return false;
}

bool run_head(cmd_t *cmd)
{
	tok_t *first = next_tok_cmd(cmd);
	if(first == NULL)
	{

		globalstate.runfor = 0;
		set_flag(FLAG_RUNNING);
	}
	else if(first->type == TOK_END)
	{
		globalstate.runfor = 100;
		set_flag(FLAG_RUNNING);

	}
	else
	{

		char *str = first->token;
		globalstate.runfor = atoi(str);
		set_flag(FLAG_RUNNING);


	}
	return true;
}
