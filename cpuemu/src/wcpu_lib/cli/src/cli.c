#include "cli.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "cliparser.h"


cli_context_t *create_cli_context()
{
	cli_context_t *ctx = calloc(1, sizeof(cli_context_t));

	ctx->lines = calloc(1, sizeof(cmdline_t));
	return ctx;
}



void pull_line(cli_context_t *ctx)
{


	ctx->lines = realloc_safe(ctx->lines, ctx->count + 1, sizeof(cmdline_t));

	char buffer[1000] = {0};
	fgets(buffer, 999, stdin );
	toklex_t *tl = lex_string(buffer);
	print_toklex(tl);
	pnode_t *head = evaluate_lex(tl);

	ctx->lines[ctx->count].lex = tl;
	ctx->lines[ctx->count].head  = head;


	ctx->count++;


}
