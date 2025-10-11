#include <hashmap.h>
#include "lexer.h"
#include "parser.h"
#include "commons.h"


static tok_t *peek(parser_ctx_t *ctx)
{
	if(ctx->pos >= ctx->lex_ctx->count)
	{
		return NULL;
	}
	else
	{
		return &ctx->lex_ctx->toks[ctx->pos];
	}

}


static tok_t *next(parser_ctx_t *ctx)
{
	if(ctx->pos >= ctx->lex_ctx->count)
	{
		return NULL;
	}
	else
	{
		return &ctx->lex_ctx->toks[ctx->pos++];
	}
}

static tok_t *expect(parser_ctx_t *ctx, tok_type_t tp)
{
	tok_t *tok = next(ctx);

	if(!tok || tok->type != tp)
	{
		perror("token did not match expected");
		exit(1);
	}
	return tok;
}

parse_node_t *make_node(parse_node_kind_t kind, tok_t *tok)
{
    parse_node_t *n = CALLOC(1, parse_node_t);
    n->kind = kind;
    n->tok = tok;
    return n;
}

void add_child(parse_node_t *parent, parse_node_t *child)
{
    parent->children = REALLOC(parent->children, (parent->child_count + 1), parse_node_t *);
    parent->children[parent->child_count++] = child;
}
