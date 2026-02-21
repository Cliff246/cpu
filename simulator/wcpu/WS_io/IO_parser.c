#include "IO_parser.h"

#include "token.h"
#include "commons.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


//----------------------------------
//
//		PARSER COMMONS
//
//----------------------------------

IO_pnode_t *IO_pnode_create(tok_t *tok, IO_pnode_type_t type)
{
	//this should be done differently
	IO_pnode_t *pn = calloc(1, sizeof(IO_pnode_t));
	assert(pn != 0 && "pnode failed to create");
	pn->token = tok;
	pn->type = type;
	pn->size = 0;
	pn->alloc = 5;
	IO_pnode_t **nodes = calloc(pn->alloc, sizeof(IO_pnode_t *));
	assert(nodes);
	pn->nodes = nodes;
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

		printf("token %s %d\n", tree->token->token, tree->type);
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
	//printf("append\n");
	if(parent->size >= parent->alloc)
	{
		//printf("allocate more size\n");
		parent->alloc *= 2 + 1;
		parent->nodes = realloc_safe(parent->nodes, parent->alloc, sizeof(IO_pnode_t *));

	}



	parent->nodes[parent->size++] = child;
}

tok_t *IO_ptree_peek_tok(IO_ptree_t *tree)
{
	toklex_t *tl= tree->lex;
	tok_t *tok = &tree->lex->tokens[tree->index];
	return (tree->index < tl->tcount)? tok : NULL;

}


tok_t *IO_ptree_peek1_tok(IO_ptree_t *tree)
{
	toklex_t *tl= tree->lex;
	return (tree->index + 1 < tl->tcount)? &tree->lex->tokens[tree->index + 1] : NULL;
}

void IO_ptree_next_tok(IO_ptree_t *ctx)
{

	ctx->index++;
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

//----------------------------------
//
//		PARSER TREE
//
//----------------------------------

static IO_pnode_t *IO_ptree_value(IO_ptree_t *tree)
{
	//printf("value\n");

	tok_t *tok = IO_ptree_peek_tok(tree);
	assert(tok->type != TOK_NEWLINE);
	//print_tok(tok);
	IO_pnode_t *node = IO_pnode_create(tok, IO_PNODE_VALUE);

	return node;

}

static IO_pnode_t *IO_ptree_mono(IO_ptree_t *tree)
{

	//printf("mono\n");

	IO_pnode_t *mono = IO_pnode_create(&empty_tok, IO_PNODE_MONO);


	IO_pnode_t *value = IO_ptree_value(tree);

	IO_pnode_append(mono, value);

	return mono;
}

static IO_pnode_t *IO_ptree_list(IO_ptree_t *tree)
{
	//printf("\nlist\n");

	IO_pnode_t *list = IO_pnode_create(&empty_tok, IO_PNODE_LIST);
	while(true)
	{
		IO_ptree_next_tok(tree);

		IO_pnode_t *value = IO_ptree_value(tree);

		IO_pnode_append(list, value);
		IO_ptree_next_tok(tree);

		tok_t *sep = IO_ptree_peek_tok(tree);
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
			assert(0 && "does not equal comma or eol");
		}

	}
	return list;
}

static IO_pnode_t *IO_ptree_map(IO_ptree_t *tree)
{
	//printf("map\n");

	IO_pnode_t *map = IO_pnode_create(&empty_tok, IO_PNODE_MAP);



	while(true)
	{

		IO_ptree_next_tok(tree);
		tok_t *cur = IO_ptree_peek_tok(tree);
		//printf("map: %s\n", cur->token);

		if(cur->type == TOK_NEWLINE)
		{
			continue;
		}
		else if(cur->type == TOK_CURL)
		{
			break;
		}
		else
		{
			IO_pnode_t *entry = IO_ptree_entry(tree);
			IO_pnode_append(map, entry);

		}



	}
	IO_ptree_next_tok(tree);

	//printf("end map\n");
	return map;
}

static IO_pnode_t *IO_ptree_init(IO_ptree_t *tree)
{
	//printf("init\n");
	IO_pnode_t *init = NULL;


	tok_t *check = IO_ptree_peek_tok(tree);

	//printf("init: %s\n", check->token );

	if(check->type == TOK_CURL)
	{
		//this is deathly wrong
		init = IO_ptree_map(tree);
	}
	else if(check->type == TOK_SQUARE)
	{
		init = IO_ptree_list(tree);
	}
	else if(check->type != TOK_NEWLINE && check->type != TOK_END)
	{
		init = IO_ptree_mono(tree);
	}
	else
	{
		assert(0);
	}
	return init;
}

static IO_pnode_t *IO_ptree_name(IO_ptree_t *tree)
{
	tok_t *open_tok = IO_ptree_peek_tok(tree);
	assert(open_tok->type== TOK_BRACKET && "bracket one of name failed");
	IO_ptree_next_tok(tree);

	tok_t *name_tok = IO_ptree_peek_tok(tree);
	assert(name_tok->type == TOK_WORD && "name failed");
	IO_ptree_next_tok(tree);

	tok_t *closed_tok = IO_ptree_peek_tok(tree);
	assert(closed_tok->type == TOK_BRACKET && "bracket two of name failed");
	IO_ptree_next_tok(tree);


	IO_pnode_t *name = IO_pnode_create(name_tok, IO_PNODE_NAME);
	return name;
}

static IO_pnode_t *IO_ptree_noname(IO_ptree_t *tree)
{
	IO_pnode_t *name = IO_pnode_create( &empty_tok, IO_PNODE_NONAME);
	return name;
}

static IO_pnode_t *IO_ptree_keyword(IO_ptree_t *tree)
{
	//printf("keyword\n");
	tok_t *keyword_tok  = IO_ptree_peek_tok(tree);
	//print_tok(keyword_tok);

	assert(keyword_tok->type == TOK_WORD && "keyword is wrong");
	IO_pnode_t *keyword = IO_pnode_create(keyword_tok, IO_PNODE_KEYWORD);

	IO_ptree_next_tok(tree);
	return keyword;
}

static IO_pnode_t *IO_ptree_entry(IO_ptree_t *tree)
{
//	printf("\n");
	IO_pnode_t *entry = IO_pnode_create(&empty_tok, IO_PNODE_ENTRY);
	IO_pnode_t *keyword = IO_ptree_keyword(tree);
	tok_t *name_test_tok = IO_ptree_peek_tok(tree);
	//print_tok(name_test_tok);
	IO_pnode_t *name;
	if(name_test_tok->type == TOK_BRACKET)
	{
		//printf("name\n");
		name = IO_ptree_name(tree);
	}
	else
	{
	//	printf("noname\n");

		name = IO_ptree_noname(tree);
	}

	tok_t *equals_tok = IO_ptree_peek_tok(tree);
	assert(equals_tok->type == TOK_OP && "no equals");
	IO_ptree_next_tok(tree);

	IO_pnode_t *init = IO_ptree_init(tree);


	IO_pnode_append(entry, keyword);
	IO_pnode_append(entry, name);
	IO_pnode_append(entry, init);
	//printf("end entry\n");
	return entry;

}

bool IO_ptree_parse(IO_ptree_t *tree)
{
	assert(tree);
	toklex_t *tl = tree->lex;
	IO_pnode_t *start = IO_pnode_create(&empty_tok, IO_PNODE_NONE);
	//print_toklex(tree->lex);
	while(tree->index < tl->tcount)
	{
		tok_t *peek = IO_ptree_peek_tok(tree);
		if(peek->type != TOK_NEWLINE && peek->type != TOK_END)
		{

			IO_pnode_t *entry = IO_ptree_entry(tree);
			IO_pnode_append(start, entry);
		}

		else
		{
			IO_ptree_next_tok(tree);
		}
	}
	tree->head = start;
	return false;
}


void IO_pnode_free(IO_pnode_t *node)
{
	if(!node)
		return;




	for(int i = 0; i < node->size; ++i)
	{
		IO_pnode_free(node->nodes[i]);
	}

	free(node->nodes);
	free(node);
}

void IO_ptree_free(IO_ptree_t *tree)
{
	IO_pnode_free(tree->head);
	free_toklex(tree->lex);
	free(tree);

}