#include "SYNTAX_parser.h"


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

SYNTAX_pnode_t *SYNTAX_pnode_create(tok_t *tok, SYNTAX_pnode_type_t type)
{
	//this should be done differently
	SYNTAX_pnode_t *pn = calloc(1, sizeof(SYNTAX_pnode_t));
	assert(pn != 0 && "pnode failed to create");
	pn->token = tok;
	pn->type = type;
	pn->size = 0;
	pn->alloc = 5;
	SYNTAX_pnode_t **nodes = calloc(pn->alloc, sizeof(SYNTAX_pnode_t *));
	assert(nodes);
	pn->nodes = nodes;
	return pn;
}




void SYNTAX_pnode_print(SYNTAX_pnode_t *tree, int depth)
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
		SYNTAX_pnode_print(tree->nodes[i], depth + 1);
	}

}

void SYNTAX_pnode_append(SYNTAX_pnode_t *parent, SYNTAX_pnode_t *child)
{
	if(child == NULL)
	{
		return;
	}
	//printf("append\n");
	if(parent->size >= parent->alloc)
	{
		//printf("allocate more size\n");
		parent->alloc = (2 * parent->alloc) + 1;
		parent->nodes = realloc_safe(parent->nodes, parent->alloc, sizeof(SYNTAX_pnode_t *));

	}



	parent->nodes[parent->size++] = child;
}

tok_t *SYNTAX_ptree_peek_tok(SYNTAX_ptree_t *tree)
{
	toklex_t *tl= tree->lex;
	tok_t *tok = &tree->lex->tokens[tree->index];
	return (tree->index < tl->tcount)? tok : NULL;

}


tok_t *SYNTAX_ptree_peek1_tok(SYNTAX_ptree_t *tree)
{
	toklex_t *tl= tree->lex;
	return (tree->index + 1 < tl->tcount)? &tree->lex->tokens[tree->index + 1] : NULL;
}

void SYNTAX_ptree_next_tok(SYNTAX_ptree_t *ctx)
{

	ctx->index++;
}


tok_t *SYNTAX_ptree_expect_tok(SYNTAX_ptree_t *tree, tok_type_t type)
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



SYNTAX_ptree_t *SYNTAX_ptree_create(toklex_t *tl)
{

	SYNTAX_ptree_t *tree = calloc(1, sizeof(SYNTAX_ptree_t));
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

static SYNTAX_pnode_t *SYNTAX_ptree_value(SYNTAX_ptree_t *tree)
{
	//printf("value\n");

	tok_t *tok = SYNTAX_ptree_peek_tok(tree);
	assert(tok->type != TOK_NEWLINE);
	//print_tok(tok);
	SYNTAX_pnode_t *node = SYNTAX_pnode_create(tok, SYNTAX_PNODE_VALUE);

	return node;

}

static SYNTAX_pnode_t *SYNTAX_ptree_mono(SYNTAX_ptree_t *tree)
{

	//printf("mono\n");

	SYNTAX_pnode_t *mono = SYNTAX_pnode_create(&empty_tok, SYNTAX_PNODE_MONO);


	SYNTAX_pnode_t *value = SYNTAX_ptree_value(tree);

	SYNTAX_pnode_append(mono, value);

	return mono;
}

static SYNTAX_pnode_t *SYNTAX_ptree_list(SYNTAX_ptree_t *tree)
{
	//printf("\nlist\n");

	SYNTAX_pnode_t *list = SYNTAX_pnode_create(&empty_tok, SYNTAX_PNODE_LIST);
	while(true)
	{
		SYNTAX_ptree_next_tok(tree);

		SYNTAX_pnode_t *value = SYNTAX_ptree_value(tree);

		SYNTAX_pnode_append(list, value);
		SYNTAX_ptree_next_tok(tree);

		tok_t *sep = SYNTAX_ptree_peek_tok(tree);
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

static SYNTAX_pnode_t *SYNTAX_ptree_map(SYNTAX_ptree_t *tree)
{
	//printf("map\n");

	SYNTAX_pnode_t *map = SYNTAX_pnode_create(&empty_tok, SYNTAX_PNODE_MAP);



	while(true)
	{

		SYNTAX_ptree_next_tok(tree);
		tok_t *cur = SYNTAX_ptree_peek_tok(tree);
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
			SYNTAX_pnode_t *entry = SYNTAX_ptree_entry(tree);
			SYNTAX_pnode_append(map, entry);

		}



	}
	SYNTAX_ptree_next_tok(tree);

	//printf("end map\n");
	return map;
}

static SYNTAX_pnode_t *SYNTAX_ptree_init(SYNTAX_ptree_t *tree)
{
	//printf("init\n");
	SYNTAX_pnode_t *init = NULL;


	tok_t *check = SYNTAX_ptree_peek_tok(tree);

	//printf("init: %s\n", check->token );

	if(check->type == TOK_CURL)
	{
		//this is deathly wrong
		init = SYNTAX_ptree_map(tree);
	}
	else if(check->type == TOK_SQUARE)
	{
		init = SYNTAX_ptree_list(tree);
	}
	else if(check->type != TOK_NEWLINE && check->type != TOK_END)
	{
		init = SYNTAX_ptree_mono(tree);
	}
	else
	{
		assert(0);
	}
	return init;
}

static SYNTAX_pnode_t *SYNTAX_ptree_name(SYNTAX_ptree_t *tree)
{
	tok_t *open_tok = SYNTAX_ptree_peek_tok(tree);
	assert(open_tok->type== TOK_BRACKET && "bracket one of name failed");
	SYNTAX_ptree_next_tok(tree);

	tok_t *name_tok = SYNTAX_ptree_peek_tok(tree);
	assert(name_tok->type == TOK_WORD && "name failed");
	SYNTAX_ptree_next_tok(tree);

	tok_t *closed_tok = SYNTAX_ptree_peek_tok(tree);
	assert(closed_tok->type == TOK_BRACKET && "bracket two of name failed");
	SYNTAX_ptree_next_tok(tree);


	SYNTAX_pnode_t *name = SYNTAX_pnode_create(name_tok, SYNTAX_PNODE_NAME);
	return name;
}

static SYNTAX_pnode_t *SYNTAX_ptree_noname(SYNTAX_ptree_t *tree)
{
	SYNTAX_pnode_t *name = SYNTAX_pnode_create( &empty_tok, SYNTAX_PNODE_NONAME);
	return name;
}

static SYNTAX_pnode_t *SYNTAX_ptree_keyword(SYNTAX_ptree_t *tree)
{
	//printf("keyword\n");
	tok_t *keyword_tok  = SYNTAX_ptree_peek_tok(tree);
	//print_tok(keyword_tok);

	assert(keyword_tok->type == TOK_WORD && "keyword is wrong");
	SYNTAX_pnode_t *keyword = SYNTAX_pnode_create(keyword_tok, SYNTAX_PNODE_KEYWORD);

	SYNTAX_ptree_next_tok(tree);
	return keyword;
}

static SYNTAX_pnode_t *SYNTAX_ptree_entry(SYNTAX_ptree_t *tree)
{
//	printf("\n");
	SYNTAX_pnode_t *entry = SYNTAX_pnode_create(&empty_tok, SYNTAX_PNODE_ENTRY);
	SYNTAX_pnode_t *keyword = SYNTAX_ptree_keyword(tree);
	tok_t *name_test_tok = SYNTAX_ptree_peek_tok(tree);
	//print_tok(name_test_tok);
	SYNTAX_pnode_t *name;
	if(name_test_tok->type == TOK_BRACKET)
	{
		//printf("name\n");
		name = SYNTAX_ptree_name(tree);
	}
	else
	{
	//	printf("noname\n");

		name = SYNTAX_ptree_noname(tree);
	}

	tok_t *equals_tok = SYNTAX_ptree_peek_tok(tree);
	assert(equals_tok->type == TOK_OP && "no equals");
	SYNTAX_ptree_next_tok(tree);

	SYNTAX_pnode_t *init = SYNTAX_ptree_init(tree);


	SYNTAX_pnode_append(entry, keyword);
	SYNTAX_pnode_append(entry, name);
	SYNTAX_pnode_append(entry, init);
	//printf("end entry\n");
	return entry;

}

bool SYNTAX_ptree_parse(SYNTAX_ptree_t *tree)
{
	assert(tree);
	toklex_t *tl = tree->lex;
	SYNTAX_pnode_t *start = SYNTAX_pnode_create(&empty_tok, SYNTAX_PNODE_NONE);
	//print_toklex(tree->lex);
	while(tree->index < tl->tcount)
	{
		tok_t *peek = SYNTAX_ptree_peek_tok(tree);
		if(peek->type != TOK_NEWLINE && peek->type != TOK_END)
		{

			SYNTAX_pnode_t *entry = SYNTAX_ptree_entry(tree);
			SYNTAX_pnode_append(start, entry);
		}

		else
		{
			SYNTAX_ptree_next_tok(tree);
		}
	}
	tree->head = start;
	return false;
}


void SYNTAX_pnode_free(SYNTAX_pnode_t *node)
{
	if(!node)
		return;




	for(int i = 0; i < node->size; ++i)
	{
		SYNTAX_pnode_free(node->nodes[i]);
	}

	free(node->nodes);
	free(node);
}

void SYNTAX_ptree_free(SYNTAX_ptree_t *tree)
{
	SYNTAX_pnode_free(tree->head);
	free_toklex(tree->lex);
	free(tree);

}