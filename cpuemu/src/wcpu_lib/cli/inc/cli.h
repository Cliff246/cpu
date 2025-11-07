#ifndef __CPU_CLI__
#define __CPU_CLI__

#include "token.h"
#include "cmdsys.h"
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
	cmdline_t *line;




}cmd_t;

typedef struct cli_context
{

	cmd_t *commands;
	size_t count, alloc;

}cli_context_t;

char *read_line(void);

cmdline_t *create_line(char *str);

cli_context_t *create_cli_context();





cmd_t *create_command(cli_context_t* ctx, cmdline_t *cmdline);

void pull_line(cli_context_t *ctx);




#endif
