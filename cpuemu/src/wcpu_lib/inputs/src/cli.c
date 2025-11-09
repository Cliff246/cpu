#include "cli.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "cliparser.h"

void create_cli_context(cli_context_t *ctx)
{


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
		//print_toklex(tl);
		cl->lex = tl;
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

cmdline_t create_line(char *str)
{
	cmdline_t cl = {0};
	lex_cmdline(&cl, str);
	//parse_cmdline(&cl);
	cl.str = str;
	return cl;

	if(cl.parsed)
	{
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

cmd_t *create_command(cli_context_t* ctx, cmdline_t cmdline)
{
	if(ctx->count >= ctx->alloc)
		ctx->commands = realloc_safe(ctx->commands, ctx->alloc *= 2, sizeof(cmd_t));


	cmd_t *cmd = &ctx->commands[ctx->count++];
	cmd->line = cmdline;
	cmd->token = 0;
	return cmd;
}

cmd_t *pull_line(cli_context_t *ctx)
{
	return create_command(ctx, create_line(read_line()));
}


char *get_elem_line(cmdline_t *line, int i)
{
	if(line->lex->slen > i && i >= 0)
	{
		return line->lex->tokens[i].token;
	}
	return NULL;
}

tok_t *get_tok_cmd(cmd_t *cmd, int i)
{
	if(cmd->line.lex->slen > i && i >= 0)
	{
		return &cmd->line.lex->tokens[i];
	}
	else
	{
		return NULL;
	}
}

tok_t *next_tok_cmd(cmd_t *cmd)
{
	return get_tok_cmd(cmd, cmd->token++);
}
cmdpacket_t create_cmdpacket(cmdline_t line)
{







}
