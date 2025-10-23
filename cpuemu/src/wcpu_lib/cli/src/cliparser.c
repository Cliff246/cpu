#include "cmdsys.h"
#include "cli.h"
#include "cliparser.h"
#include "token.h"
#include "common.h"
#include <stdio.h>
#include <string.h>

static pnode_t *create_pnode(tok_t *tok, pnode_type_t type)
{
	pnode_t *pn = calloc(1, sizeof(pnode_t));

	pn->token = tok;
	pn->type = type;
	pn->length = 0;
	pn->nodes = NULL;
	return pn;
}

static void print_pnode_tree(pnode_t *tree, int depth)
{
	if(!tree)
		return;


	for(int i = 0; i < depth; ++i)
	{
		printf("\t");
	}
	if(tree->token != NULL)
	{

		printf("token %s\n", tree->token->token);
	}
	for(int i = 0; i < tree->length; ++i)
	{
		print_pnode_tree(tree->nodes[i], depth + 1);
	}

}

static void append_pnode(pnode_t *parent, pnode_t *child)
{
	if(child == NULL)
	{
		return;
	}
	parent->nodes = realloc_safe(parent->nodes, parent->length + 1, sizeof(pnode_t *));

	parent->nodes[parent->length++] = child;
}

static tok_t *peek_tok(pcontext_t *ctx)
{
	return &ctx->lex->tokens[ctx->index];
}


static tok_t *next_tok(pcontext_t *ctx)
{

	return &ctx->lex->tokens[ctx->index++];
}

static tok_t *expect_tok(pcontext_t *ctx, tok_type_t type)
{
	tok_t *tok = &ctx->lex->tokens[ctx->index++];
	if(tok->type == type)
	{
		return tok;
	}
	else
	{
		return NULL;
	}
}


pnode_t *calculate_factor(pcontext_t *ctx)
{
	tok_t *tok = expect_tok(ctx, TOK_INT);
	printf("%s %d\n",tok->token, tok->type);
	return create_pnode(tok, PN_NUM);
}

pnode_t *calculate_term1(pcontext_t *ctx)
{

	tok_t *op = expect_tok(ctx, TOK_OP);
	if(op == NULL)
	{
		return NULL;
	}
	printf("term: %s\n", op->token);
	pnode_t *head = create_pnode(NULL, PN_EMPTY);
	if(!strcmp(op->token, "*") )
	{

		pnode_t *mul = create_pnode(op, PN_MUL);

		pnode_t *factor = calculate_factor(ctx);
		if(factor == NULL)
			return head;
		pnode_t *term1 = calculate_term1(ctx);

		if(term1 == NULL)
			return head;
		append_pnode(head, mul);
		append_pnode(head, factor);
		append_pnode(head, term1);
		return head;

	}

	if(!strcmp(op->token, "/") )
	{
		pnode_t *div = create_pnode(op, PN_DIV);
		pnode_t *factor = calculate_factor(ctx);
		if(factor == NULL)
			return head;
		pnode_t *term1 = calculate_term1(ctx);
		if(term1 == NULL)
			return head;
		append_pnode(head, div);
		append_pnode(head, factor);
		append_pnode(head, term1);
		return head;
	}
	return head;
}

pnode_t *calculate_term(pcontext_t *ctx)
{
	pnode_t *head = create_pnode(NULL, PN_EMPTY);
	pnode_t *factor = calculate_factor(ctx);
	pnode_t *term1 = calculate_term1(ctx);
	if(factor == NULL || term1 == NULL)
		return NULL;
	append_pnode(head, factor);
	append_pnode(head, term1);
	return head;
}

pnode_t *calculate_expr1(pcontext_t *ctx)
{
	printf("token 1\n");
	pnode_t *head = create_pnode(NULL, PN_OP);
	tok_t *next = expect_tok(ctx, TOK_OP);
	printf("token %s\n", next->token);
	if(!next)
		return NULL;

	printf("next = %s\n", next->token);
	if(!strcmp(next->token, "+"))
	{
		pnode_t *add = create_pnode(next, PN_ADD);

		pnode_t *term = calculate_term(ctx);
		pnode_t *expr1 = calculate_expr1(ctx);
		append_pnode(head, add);
		append_pnode(head, term);
		append_pnode(head, expr1);
	}
	else if(!strcmp(next->token, "-"))
	{
		pnode_t *sub = create_pnode(next, PN_SUB);

		pnode_t *term = calculate_term(ctx);

		pnode_t *expr1 = calculate_expr1(ctx);
		append_pnode(head, sub);
		append_pnode(head, term);
		append_pnode(head, expr1);
	}
	return head;
}

pnode_t *calculate_expr(pcontext_t *ctx)
{
	pnode_t *exp = create_pnode(NULL, PN_OP);

	pnode_t *term = calculate_term(ctx);
	pnode_t *exp1 = calculate_expr1(ctx);
	append_pnode(exp, term);
	append_pnode(exp, exp1);
	return exp;
}



pnode_t *evaluate_lex(toklex_t *lex)
{
	pcontext_t ctx = {.lex = lex, .index = 0};

	pnode_t *head = create_pnode(NULL, PN_START);



	while(ctx.index < lex->tcount)
	{
		printf("repeater %d\n", peek_tok(&ctx)->type);
		if(peek_tok(&ctx)->type == TOK_INT)
		{
			append_pnode(head, calculate_expr(&ctx));

		}
		else
		{

			next_tok(&ctx);
		}
	}

	print_pnode_tree(head, 0);
	return head;
}


