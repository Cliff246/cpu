#include "cli.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "cliparser.h"


cli_context_t *create_cli_context()
{
	cli_context_t *ctx = calloc(1, sizeof(cli_context_t));


	ctx->alloc = 10;
	ctx->count = 0;
	ctx->commands = calloc(ctx->alloc, sizeof(cmd_t ));

}

void lex_cmdline(cmdline_t *cl, char *str)
{
	toklex_t *tl = lex_string(str);
	if(tl == NULL)
	{
		cl->lexed = false;
	}
	else
	{
		cl->lex = lex_string(str);
		cl->lexed = true;
	}
}

void parse_cmdline(cmdline_t *cl)
{
	if(cl->lexed)
	{
		pnode_t *node = evaluate_lex(cl->lex);
		if(!node)
		{
			cl->parsed = false;
		}
		else
		{
			cl->head = node;
			cl->parsed = true;
		}
	}
	else
		return;
}

cmdline_t *create_line(char *str)
{
	cmdline_t *cl = calloc(1, sizeof(cmdline_t));

	lex_cmdline(cl, str);
	parse_cmdline(cl);
	cl->str = str;
	if(cl->parsed)
	{
		return cl;
	}
	else
	{

	}
}

char *read_line(void)
{
	char *buffer = calloc(1001, sizeof(char));
	fgets(buffer, 1000, stdin);
	return buffer;
}

cmd_t *create_command(cli_context_t* ctx, cmdline_t *cmdline)
{
	if(ctx->count >= ctx->alloc)
		ctx->commands = realloc_safe(ctx->commands, ctx->alloc *= 2, sizeof(cmd_t));


	cmd_t *cmd = &ctx->commands[ctx->count++];
	cmd->line = cmdline;

	return cmd;
}

void pull_line(cli_context_t *ctx)
{
	create_command(ctx, create_line(read_line()));








}
