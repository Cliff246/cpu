#ifndef __CPU_CLI__
#define __CPU_CLI__

#include "token.h"
#include "cliparser.h"

#include <stdbool.h>
#include <stdlib.h>






typedef struct cmdline
{
	pnode_t *head;
	toklex_t *lex;
	char *str;
	bool parsed;
	bool lexed;

}cmdline_t;





typedef struct command
{
	cmdline_t line;

	int token;


}cmd_t;


typedef struct cli_context
{

	cmd_t *commands;
	size_t count, alloc;

}cli_context_t;


char *get_elem_line(cmdline_t *line, int i);
tok_t *get_tok_cmd(cmd_t *cmd, int i);
tok_t *next_tok_cmd(cmd_t *cmd);
tok_t *peek_tok_cmd(cmd_t *cmd);



char *read_line(void);

cmdline_t create_line(char *str);

void create_cli_context(cli_context_t *ctx);




cmd_t *create_command(cli_context_t* ctx, cmdline_t cmdline);

cmd_t *pull_line(cli_context_t *ctx);




#endif
