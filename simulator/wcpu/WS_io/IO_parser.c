#include "IO_parser.h"

#include "token.h"
#include "commons.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

IO_pnode_t *IO_pnode_create(tok_t *tok, IO_pnode_type_t type)
{
	//this should be done differently
	IO_pnode_t *pn = calloc(1, sizeof(IO_pnode_t));
	assert(pn != 0 && "pnode failed to create");
	pn->token = tok;
	pn->type = type;
	pn->size = 0;
	pn->nodes = NULL;
	return pn;
}




void IO_pnode_print(IO_pnode_t *tree, int depth)
{
	if(!tree)
		return;


	for(int i = 0; i < depth; ++i)
	{
		printf("\t");
	}
	if(tree->token != NULL)
	{

		printf("token %s %d\n", tree->token->token, tree->token->type);
	}
	for(int i = 0; i < tree->size; ++i)
	{
		IO_pnode_print(tree->nodes[i], depth + 1);
	}

}

void IO_pnode_append(IO_pnode_t *parent, IO_pnode_t *child)
{
	if(child == NULL)
	{
		return;
	}
	parent->nodes = realloc_safe(parent->nodes, parent->size + 1, sizeof(IO_pnode_t *));

	parent->nodes[parent->size++] = child;
}

tok_t *IO_ptree_peek_tok(IO_ptree_t *tree)
{
	toklex_t *tl= tree->lex;
	return (tree->index < tl->tcount)? &tree->lex->tokens[tree->index] : NULL;

}


tok_t *IO_ptree_peek1_tok(IO_ptree_t *tree)
{
	toklex_t *tl= tree->lex;
	return (tree->index + 1 < tl->tcount)? &tree->lex->tokens[tree->index + 1] : NULL;
}

tok_t *IO_ptree_next_tok(IO_ptree_t *ctx)
{
	return &ctx->lex->tokens[++ctx->index];
}


tok_t *IO_ptree_expect_tok(IO_ptree_t *tree, tok_type_t type)
{
	tok_t *tok = &tree->lex->tokens[tree->index++];
	if(tok->type == type)
	{
		return tok;
	}
	else
	{
		return NULL;
	}
}



IO_ptree_t *IO_ptree_create(toklex_t *tl)
{

	IO_ptree_t *tree = calloc(1, sizeof(IO_ptree_t));
	assert(tree);
	tree->lex = tl;
	tree->index = 0;
	tree->head = NULL;
	return tree;
}



static IO_pnode_t *IO_pnode_set(IO_ptree_t *tree)
{


	IO_pnode_t *pnode = IO_pnode_create(&empty_tok, IO_PNODE_SET);
	tok_t *keytok = IO_ptree_peek_tok(tree);
	assert(keytok);
	IO_pnode_t *key  = IO_pnode_create(keytok, IO_PNODE_KEY);

	IO_pnode_t *first = NULL;
	tok_t *first_tok = IO_ptree_next_tok(tree);
	if(first_tok->type == TOK_OP)
	{
		tok_t *next = IO_ptree_next_tok(tree);
		if(next->type == TOK_NEWLINE || next->type == TOK_END)
		{
			assert(0);
		}
		first = IO_pnode_create(next, IO_PNODE_VALUE);

	}
	else if(first_tok->type == TOK_SQUARE)
	{
		first = IO_pnode_create(&empty_tok, IO_PNODE_LIST);
		while(true)
		{
			tok_t *iter = IO_ptree_next_tok(tree);
			IO_pnode_t *elem = IO_pnode_create(iter,  IO_PNODE_VALUE);
			IO_pnode_append(first, elem);
			tok_t *sep = IO_ptree_next_tok(tree);
			if(sep->type == TOK_NEWLINE)
			{
				assert(0);
			}
			if(sep->type == TOK_SQUARE)
			{
				break;
			}
			else if(sep->type != TOK_COMMA)
			{
				assert(0);
			}

		}
	}
	else
	{
		fprintf(stderr, "cannot use: %s", first_tok->token);
		exit(EXIT_FAILURE);
	}






	IO_pnode_append(pnode, key);
	IO_pnode_append(pnode, first);


	return pnode;
}

static IO_pnode_t *IO_pnode_body(IO_ptree_t *tree)
{
	IO_pnode_t *body = IO_pnode_create(&empty_tok, IO_PNODE_BODY);
	while(true)
	{
		tok_t *tok = IO_ptree_next_tok(tree);
		if(!tok)
			break;
		if(tok->type == TOK_CURL || tok->type == TOK_END)
		{
			break;
		}
		if(tok->type == TOK_NEWLINE)
		{
			continue;
		}



		IO_pnode_t *set =  IO_pnode_set(tree);
		IO_pnode_append(body, set);

	}


	return body;

}

static IO_pnode_t *IO_pnode_header(IO_ptree_t *tree)
{
	assert(tree);
	tok_t *header_name = IO_ptree_expect_tok(tree, TOK_STRING);
	if(!header_name)
	{
		fprintf(stderr, "header colon failed\n");
		assert(0);
		exit(1);
	}
	tok_t *colon = IO_ptree_expect_tok(tree, TOK_COLON);
	if(!colon)
	{
		fprintf(stderr, "header colon failed %s\n", header_name->token);
		assert(0);
		exit(1);
	}
	IO_pnode_t *header = IO_pnode_create(header_name, IO_PNODE_HEADER);



	while(true)
	{
		tok_t *next = IO_ptree_next_tok(tree);

		if(next->type == TOK_SEMICOLON)
		{
			break;
		}
		else if(next->type == TOK_CURL)
		{
			IO_pnode_t *body = IO_pnode_body(tree);

			//IO_pnode_print(body, 0);
			IO_pnode_append(header, body);

		}
		else if(next->type == TOK_NEWLINE)
		{
			continue;
		}
		else
		{
			break;
		}

	}



	return header;

}

bool IO_ptree_parse(IO_ptree_t *tree)
{
	assert(tree);
	toklex_t *tl = tree->lex;
	IO_pnode_t *base =  IO_pnode_create(&empty_tok, IO_PNODE_START);
	while(tree->index < tl->tcount)
	{
		tok_t *peek = IO_ptree_peek_tok(tree);
		if(peek->type == TOK_STRING)
		{

			IO_pnode_t *header =  IO_pnode_header(tree);

			IO_pnode_append(base, header);
		}
		else
		{
			IO_ptree_next_tok(tree);
		}
	}

	tree->head = base;
	return false;
}


/*
IO_pnode_t *calculate_factor(pcontext_t *ctx)
{
	tok_t *tok = expect_tok(ctx, TOK_INT);
	printf("%s %d\n",tok->token, tok->type);
	return create_pnode(tok, PN_NUM);
}

IO_pnode_t *calculate_term1(pcontext_t *ctx)
{

	tok_t *op = expect_tok(ctx, TOK_OP);
	if(op == NULL)
	{
		return NULL;
	}
	printf("term: %s\n", op->token);
	IO_pnode_t *head = create_pnode(NULL, PN_EMPTY);
	if(!strcmp(op->token, "*") )
	{

		IO_pnode_t *mul = create_pnode(op, PN_MUL);

		IO_pnode_t *factor = calculate_factor(ctx);
		if(factor == NULL)
			return head;
		IO_pnode_t *term1 = calculate_term1(ctx);

		if(term1 == NULL)
			return head;
		append_pnode(head, mul);
		append_pnode(head, factor);
		append_pnode(head, term1);
		return head;

	}

	if(!strcmp(op->token, "/") )
	{
		IO_pnode_t *div = create_pnode(op, PN_DIV);
		IO_pnode_t *factor = calculate_factor(ctx);
		if(factor == NULL)
			return head;
		IO_pnode_t *term1 = calculate_term1(ctx);
		if(term1 == NULL)
			return head;
		append_pnode(head, div);
		append_pnode(head, factor);
		append_pnode(head, term1);
		return head;
	}
	return head;
}

IO_pnode_t *calculate_term(pcontext_t *ctx)
{
	IO_pnode_t *head = create_pnode(NULL, PN_EMPTY);
	IO_pnode_t *factor = calculate_factor(ctx);
	IO_pnode_t *term1 = calculate_term1(ctx);
	if(factor == NULL || term1 == NULL)
		return NULL;
	append_pnode(head, factor);
	append_pnode(head, term1);
	return head;
}

IO_pnode_t *calculate_expr1(pcontext_t *ctx)
{
	//printf("token 1\n");
	IO_pnode_t *head = create_pnode(NULL, PN_OP);
	tok_t *next = expect_tok(ctx, TOK_OP);
	//printf("token %s\n", next->token);
	if(!next)
		return NULL;

	//printf("next = %s\n", next->token);
	if(!strcmp(next->token, "+"))
	{
		IO_pnode_t *add = create_pnode(next, PN_ADD);

		IO_pnode_t *term = calculate_term(ctx);
		IO_pnode_t *expr1 = calculate_expr1(ctx);
		append_pnode(head, add);
		append_pnode(head, term);
		append_pnode(head, expr1);
	}
	else if(!strcmp(next->token, "-"))
	{
		IO_pnode_t *sub = create_pnode(next, PN_SUB);

		IO_pnode_t *term = calculate_term(ctx);

		IO_pnode_t *expr1 = calculate_expr1(ctx);
		append_pnode(head, sub);
		append_pnode(head, term);
		append_pnode(head, expr1);
	}
	return head;
}

IO_pnode_t *calculate_expr(pcontext_t *ctx)
{
	IO_pnode_t *exp = create_pnode(NULL, PN_OP);

	IO_pnode_t *term = calculate_term(ctx);
	IO_pnode_t *exp1 = calculate_expr1(ctx);
	append_pnode(exp, term);
	append_pnode(exp, exp1);
	return exp;
}



IO_pnode_t *evaluate_lex(toklex_t *lex)
{
	pcontext_t ctx = {.lex = lex, .index = 0};

	IO_pnode_t *head = create_pnode(&empty_tok, PN_START);


	while(ctx.index < lex->tcount)
	{
		//printf("repeater %d\n", peek_tok(&ctx)->type);
		tok_t *tok = next_tok(&ctx);

		tok_type_t type = tok->type;


		if(type == TOK_WORD)
		{
			append_pnode(head, create_pnode(tok, PN_KEYWORD));
		}
		else if(type == TOK_INT)
		{
			append_pnode(head, calculate_expr(&ctx));
		}
	}

	print_IO_pnode_tree(head, 0);
	return head;
}

*/
