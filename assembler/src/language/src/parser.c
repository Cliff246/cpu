#include <hashmap.h>
#include "lexer.h"
#include "parser.h"
#include "commons.h"


void print_depth(parse_node_t *node, int depth)
{


	for(int d = 0; d < depth; ++d)
	{
		printf("\t");
	}
	if(node->tok == NULL)
	{
		printf("\n");
	}
	else
	{
		if(node->tok->lexeme == NULL)
		{
			printf("token is invalid during parse %d\n", depth);
			exit(1);
		}
		printf("lexme %s %d\n", node->tok->lexeme, node->kind);

	}
	//printf("%s %d\n", node->tok->lexeme, node->kind);

	if(node->child_count > 0)
	{
		for(int i = 0; i < node->child_count; ++i)
		{

			print_depth(node->children[i], depth + 1);
		}
	}

}

void free_parse_node(parse_node_t *ptr)
{
	for(int i = 0; i < ptr->child_count; ++i)
	{
		free_parse_node(ptr->children[i]);
	}

	free_token(ptr->tok);
	free(ptr->children);
	free(ptr);

}


static tok_t *peek(parser_ctx_t *ctx)
{
	if(ctx->pos >= ctx->lex_ctx->count)
	{
		return NULL;
	}
	else
	{

		tok_t *tok = &ctx->lex_ctx->toks[ctx->pos];
		//printf("%s peek\n", tok->lexeme);
		return tok;
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

		tok_t *next =  &ctx->lex_ctx->toks[ctx->pos++];
		//print_token(next);
		return next;
	}
}

static tok_t *expect(parser_ctx_t *ctx, tok_type_t tp)
{
	tok_t *tok = next(ctx);

	if(!tok || tok->type != tp)
	{
		printf("failed expect\n");
		return NULL;
	}
	return tok;
}

parse_node_t *make_node(parse_node_kind_t kind, tok_t *tok)
{
    parse_node_t *n = CALLOC(1, parse_node_t);
    n->kind = kind;
    n->tok = tok;
	n->child_count = 0;
    return n;
}

void add_child(parse_node_t *parent, parse_node_t *child)
{
	if(!child)
	{
		printf("could not add child: %s\n", child->tok->lexeme);
		exit(1);
	}
	if(parent->child_count == 0)
	{
		parent->children = (parse_node_t **)CALLOC(1, parse_node_t *);
	}
	else
	{
    	parent->children = REALLOC(parent->children, (parent->child_count + 1), parse_node_t *);

	}
    parent->children[parent->child_count++] = child;
}


parse_node_t *parse_instruction(parser_ctx_t *ctx)
{
	//printf("instruction\n");

	parse_node_t *n = make_node(NODE_INSTR, peek(ctx));

	add_child(n, make_node(NODE_PATH, expect(ctx, TOK_TOKEN)));
	expect(ctx, TOK_DOT);

	add_child(n, make_node(NODE_SUBPATH, expect(ctx, TOK_TOKEN)));

	expect(ctx, TOK_IDENT);

	add_child(n, make_node(NODE_RD, expect(ctx, TOK_TOKEN)));

	expect(ctx, TOK_COMMA);
	add_child(n, make_node(NODE_RS1, expect(ctx, TOK_TOKEN)));

	expect(ctx, TOK_COMMA);

	add_child(n, make_node(NODE_RS2, expect(ctx, TOK_TOKEN)));

	if(peek(ctx) && peek(ctx)->type == TOK_COMMA)
	{

		next(ctx);
		add_child(n, make_node(NODE_EXPR, peek(ctx) ));
	}
	return n;
}


parser_ctx_t *create_context(lexer_ctx_t *ctx)
{
	parser_ctx_t *pctx = CALLOC(1, parser_ctx_t);
	pctx->lex_ctx = ctx;
	pctx->pos = 0;
	return pctx;
}

parse_node_t *parse_reference(parser_ctx_t *p)
{
    parse_node_t *n = make_node(NODE_REFERENCE, expect(p, TOK_REFERENCE));
    expect(p, TOK_COLON);
    return n;
}

parse_node_t *parse_segment(parser_ctx_t *p)
{

    parse_node_t *n = make_node(NODE_SEGMENT, expect(p, TOK_DIRECTIVE));
    // optionally collect args until newline
	//next(p);
    return n;
}

void parse_expr(parser_ctx_t *p, parse_node_t *parent)
{


	if(peek(p)->type == TOK_NEWLINE || !peek(p))
	{
		next(p);
		return;
	}
	else if(peek(p)->type == TOK_COMMA || peek(p)->type == TOK_IDENT)
	{
		next(p);
		parse_expr(p, parent);
	}
	else
	{
		add_child(parent, make_node(NODE_LITERAL, next(p)));
		parse_expr(p, parent);
	}



}

parse_node_t *parse_metaop(parser_ctx_t *p)
{
	if(peek(p)->type != TOK_TOKEN)
	{
		//failed mop
		parse_node_t *n = make_node(NODE_INVAL, peek(p));
		return n;
	}
	else
	{
    	parse_node_t *n = make_node(NODE_METAOP, next(p));
		parse_expr(p, n);
		return n;
	}
}

parse_node_t *parse_internal(parser_ctx_t *p)
{
	parse_node_t *child;
	tok_t *nexttok = &p->lex_ctx->toks[p->pos + 1];
	switch(nexttok->type)
	{
		case TOK_DOT:
			child = parse_instruction(p);
			break;
		case TOK_IDENT:
			printf("mop\n");
			child = parse_metaop(p);
			break;
		default:

			break;
		}
	return child;
}

parse_node_t *parse_program(parser_ctx_t *p)
{

    parse_node_t *root = make_node(NODE_PROGRAM, NULL);
	parse_node_t *seg_root = NULL;
	parse_node_t *ref_root = NULL;
	size_t max = 1000;
	int count = 0;
	bool newline = true;
    while (peek(p) && peek(p)->type != TOK_EOF && count < max)
	{
		count++;
        tok_t *t = peek(p);
        parse_node_t *child = NULL;
		//printf("current directive %p\n", seg_root);
		//printf("current label %p\n", ref_root);
		//printf("%s %d\n", t->lexeme, t->type);

		if(t->type == TOK_NEWLINE)
		{
			newline = true;
			next(p);
			continue;
		}

        else if (t->type == TOK_DIRECTIVE)
		{
            child = parse_segment(p);

			seg_root = child;
       		add_child(root, seg_root);
			newline = false;
			continue;
		}
        else if (t->type == TOK_REFERENCE)
		{
			//printf("label\n");
			child = parse_reference(p);

			ref_root = child;
       		add_child(seg_root, ref_root);
			newline = false;

			continue;
        }
		else if (t->type == TOK_IDENT && newline)
		{
			if(p->pos + 1 >= p->lex_ctx->count) break;
			tok_t *nexttok = &p->lex_ctx->toks[p->pos + 1];
			printf("%s %d\n", nexttok->lexeme, nexttok->type);
			if(nexttok->type == TOK_TOKEN)
			{
				next(p);

				child = parse_internal(p);
				add_child(ref_root, child);

			}
			else
			{
				perror("token is invalid");
			}
			newline = false;
			continue;
		}
		else
	   	{

            next(p); // skip unrecognized line
            continue;
        }
       	add_child(root, child);

    }
	print_depth(root, 0);

    return root;
}
