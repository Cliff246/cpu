#include "hydra.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void step_head(cmd_t *cmd);
void break_head(cmd_t *cmd);
void load_head(cmd_t *cmd);
void print_head(cmd_t *cmd);
void run_head(cmd_t *cmd);

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
	tok_t *first_keyword = get_tok_cmd(cmd, 0);
	if(first_keyword == NULL)
	{
		printf("must enter line");	
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
			printf("cmd not recognized");
		}
	}
	

}
