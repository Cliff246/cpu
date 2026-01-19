#include <hashmap.h>
#include "lexer.h"
#include "parser.h"
#include "commons.h"
#include "eerror.h"
#include "arguments.h"
#include "fileio.h"

struct parse_node_pair
{
	parse_node_t *a, *b;
};


static void set_parser_error(parser_ctx_t *ctx, tok_t *tok, char *error)
{

	errelm_line_t line = {.column = get_token_col(tok), .line = get_token_row(tok)};
	errelm_file_t file = {.name = get_filename_from_id(get_token_file(tok))};

	errelm_t elmline = errelm_create_line_element(line);
	errelm_t elmfile =  errelm_create_file_element(file);
	//printf("emit error\n");
	emit_error(PARSE_ERROR, error, 2, elmfile, elmline);
	//printf("emit stage2\n");
	ctx->error = true;

}


void print_depth(parse_node_t *node, int depth)
{
	if(target.debug_enabled == false)
		return;

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
			escape(1);
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


static tok_t *lookahead(parser_ctx_t *ctx)
{
	if(ctx->pos >= ctx->lex_ctx->count)
	{
		return NULL;
	}
	else
	{

		tok_t *tok = &ctx->lex_ctx->toks[ctx->pos + 1];
		//printf("%s peek\n", tok->lexeme);
		return tok;
	}
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
		//printf("failed expect\n");
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
		escape(1);
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


static void goto_next_line(parser_ctx_t *ctx)
{
	tok_t *tok = peek(ctx);
	while(tok)
	{
		if(tok->type == TOK_EOF || tok->type == TOK_NEWLINE)
			break;
		else
			tok = next(ctx);

	}
	return;
}



parse_node_t *parse_instruction(parser_ctx_t *ctx)
{
	//printf("instruction\n");

	parse_node_t *n = make_node(NODE_INSTR, peek(ctx));
	tok_t *path =  expect(ctx, TOK_TOKEN);
	if(!path)
	{
		set_parser_error(ctx, peek(ctx), "path was expected");
		goto_next_line(ctx);
		return n;
		//TODO expected token error
		//emit error recover
	}
	add_child(n, make_node(NODE_PATH, path));
	tok_t *dot = expect(ctx, TOK_DOT);

	if(!dot)
	{
		set_parser_error(ctx, peek(ctx), "dot was expected");
		goto_next_line(ctx);
		return n;
		//TODO expected token error
		//emit error recover
	}

	tok_t *subpath =  expect(ctx, TOK_TOKEN);
	if(!subpath)
	{
		set_parser_error(ctx, peek(ctx), "subpath was expected");
		goto_next_line(ctx);
		return n;
		//TODO expected token error
		//emit error and recover
	}
	add_child(n, make_node(NODE_SUBPATH, subpath));
	tok_t *ahead = peek(ctx);

	if(ahead->type == TOK_EXCLAIM)
	{
		tok_t *exclaim = expect(ctx, TOK_EXCLAIM);
		add_child(n, make_node(NODE_EXCLAIM, exclaim));

	}
	else if(ahead->type == TOK_QUESTION)
	{
		tok_t *question = expect(ctx, TOK_QUESTION);
		add_child(n, make_node(NODE_QUESTION, question));

	}


	tok_t *sep = expect(ctx, TOK_IDENT);
	if(!sep)
	{
		set_parser_error(ctx, peek(ctx), "seperator was expected");
		goto_next_line(ctx);
		return n;
		//TODO expected token error
		//emit error and recover
	}

	tok_t *rd = expect(ctx, TOK_TOKEN);
	if(!rd)
	{
		set_parser_error(ctx, peek(ctx), "rd was expected");
		goto_next_line(ctx);
		return n;		//TODO token error
		//emit error and recover
	}

	add_child(n, make_node(NODE_RD, rd));


	tok_t *comma1 = expect(ctx, TOK_COMMA);
	if(!comma1)
	{
		set_parser_error(ctx, peek(ctx), "comma was expected");
		goto_next_line(ctx);
		return n;
		//TODO expected token error
		//emit error and recover
	}

	tok_t *rs1 =  expect(ctx, TOK_TOKEN);
	if(!rs1)
	{
		set_parser_error(ctx, peek(ctx), "rs1 was expected");
		goto_next_line(ctx);
		return n;
		//TODO expected token error
		//emit error and recover
	}
	add_child(n, make_node(NODE_RS1, rs1));

	tok_t *comma2 = expect(ctx, TOK_COMMA);
	if(!comma2)
	{
		set_parser_error(ctx, peek(ctx), "comma2 was expected");
		goto_next_line(ctx);
		return n;
		//TODO expected token error
		//emit error and recover
	}

	tok_t *rs2 = expect(ctx, TOK_TOKEN );
	if(!rs2)
	{
		set_parser_error(ctx, peek(ctx), "rs2 was expected");
		goto_next_line(ctx);
		return n;
		//TODO expected token error
		//emit error and recover
	}


	add_child(n, make_node(NODE_RS2, rs2));
	if(!peek(ctx))
	{
		return n;
	}
	if(peek(ctx)->type == TOK_COMMA)
	{

		next(ctx);
		//TODO fix this
		//this should actually compute an expression
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

struct parse_node_pair parse_reference(parser_ctx_t *p)
{
	tok_t *current = peek(p);
	tok_t *reference = expect(p, TOK_REFERENCE);
	if(!(reference))
	{
		//TODO emit expected token error
		LOG("parsed reference failed", 0);
		escape(1);
		//emit error and recover
	}
    parse_node_t *n = make_node(NODE_REFERENCE, reference);


	tok_t *colon = expect(p, TOK_COLON);
	if(!colon)
	{
		LOG("expected colon\n", 0);
		escape(1);
		//TODO
		//this should be a colon if not that's bad
	}

	parse_node_t *arguments = make_node(NODE_ARGS, colon);
	add_child(n, arguments);
	parse_node_t *instructions = make_node(NODE_REFERENCE_SCOPE, &empty_token);
	add_child(n, instructions);

	struct parse_node_pair pair = {.a = n, .b = instructions};

	while(true)
	{
		tok_t *ahead = peek(p);
		if(ahead == NULL)
		{
			return pair;
		}
		if(ahead->type == TOK_EOF || ahead->type == TOK_COMMENT || ahead->type == TOK_NEWLINE)
		{
			return pair;
		}
		else
		{
			if(ahead->type == TOK_IDENT)
			{
				next(p);
			}
			else if(ahead->type == TOK_TOKEN)
			{
				parse_node_t *extra = make_node(NODE_ARGS, expect(p, TOK_TOKEN));
				add_child(arguments, extra);
			}
			else
			{
				//
				perror("invalid type");
				return pair;
			}
		}
	}


	//TODO fill more

    return pair;

}

struct parse_node_pair parse_segment(parser_ctx_t *p)
{

	tok_t *segment = expect(p, TOK_SEGMENT);
	if(!segment)
	{
		//TODO expected token error
	}
    parse_node_t *n = make_node(NODE_SCOPE, segment );
    // optionally collect args until newline

	parse_node_t *arghead = make_node(NODE_ARGS, segment);
	parse_node_t *refhead = make_node(NODE_SEGMENT, segment);
	//in this order
	add_child(n, arghead);
	add_child(n, refhead);
	struct parse_node_pair pair = {.a = n, .b = refhead};
	if(peek(p)->type == TOK_IDENT)
	{
		while(true)
		{
			tok_t *cur  =next(p);

			if(cur->type == TOK_TOKEN)
			{
				parse_node_t *argument = make_node(NODE_ARGS, cur);
				add_child(arghead, argument);

			}
			else if(cur->type == TOK_IDENT)
			{
				continue;
			}
			else
				break;

		}

	}

	return pair;


	//next(p);
}

void parse_expr(parser_ctx_t *p, parse_node_t *parent)
{


	if(peek(p)->type == TOK_NEWLINE || !peek(p) || peek(p)->type == TOK_EOF)
	{
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

parse_node_t *parse_directive(parser_ctx_t *p)
{
	parse_node_t *n = make_node(NODE_DIRECTIVE, next(p));
	parse_expr(p, n);
	if(peek(p)->type == TOK_IDENT)
	{
		while(true)
		{
			tok_t *cur  =next(p);

			if(cur->type != TOK_EOF || cur->type != TOK_NEWLINE)
			{
				parse_node_t *argument = make_node(NODE_ARGS, cur);
				add_child(n, argument);

			}
			else
			{
				break;
			}

		}

	}
	return n;
}

parse_node_t *parse_internal(parser_ctx_t *p)
{
	parse_node_t *child;
	tok_t *nexttok = lookahead(p);
	if(nexttok == NULL)
	{

		return NULL;
	}
	switch(nexttok->type)
	{
		case TOK_DOT:
			child = parse_instruction(p);
			break;
		case TOK_IDENT:
			child = parse_metaop(p);
			break;

		default:
			//emit error and skip to next valid internal consuming all
			break;
	}

	//should try to skip this if child is bad


	return child;
}

parse_node_t *parse_program(parser_ctx_t *p)
{

    parse_node_t *root = make_node(NODE_PROGRAM, NULL);
	parse_node_t *seg_root = NULL;
	parse_node_t *ref_root = NULL;

	int count = 0;
	bool newline = true;

    while (peek(p) && peek(p)->type != TOK_EOF)
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
		else if (t->type == TOK_PERCENT)
		{

            child = parse_directive(p);

       		add_child(root, child);
			newline = false;
			continue;
		}
        else if (t->type == TOK_SEGMENT)
		{



           	struct parse_node_pair seg_pair = parse_segment(p);
			seg_root = seg_pair.b;
       		add_child(root, seg_pair.a);
			newline = false;
			continue;
		}
        else if (t->type == TOK_REFERENCE)
		{

			if(!seg_root)
			{
				//TODO segment is not defined before instruction, must exit
				//EMIT instruction and try to find a segment.
			}

			//printf("label\n");

			//should come in
			//a = reference ptr
			//b = instruction scope ptr
			struct parse_node_pair pair  = parse_reference(p);
			ref_root = pair.b;

       		add_child(seg_root, pair.a);
			newline = false;

			continue;
        }
		else if (t->type == TOK_IDENT && newline)
		{




			if(p->pos + 1 >= p->lex_ctx->count) break;
			tok_t *nexttok = lookahead(p);
			//printf("%s %d\n", nexttok->lexeme, nexttok->type);
			if(nexttok->type == TOK_TOKEN)
			{
				next(p);

				child = parse_internal(p);
				if(child == NULL)
				{
					newline = false;
					continue;
				}

				if(seg_root && !ref_root)
				{
					add_child(seg_root, child);
				}
				else if(ref_root)
				{
					add_child(ref_root, child);

				}
				else
				{
					//should die
					set_parser_error(p, peek(p), "should not be outside of any elements");
					goto_next_line(p);

				}


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
