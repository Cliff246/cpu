#include "MANFST_entry.h"
#include "MANFST_prototag.h"
#include "SYNTAX_parser.h"
#include "TAG_tag.h"
#include "commons.h"
#include "token.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//returns TAG_type_t or -1 if not all the same

MANFST_prototag_t *MANFST_init_entry_tag(SYNTAX_pnode_t *head);

void MANFST_free_entry(MANFST_entry_t *entry);
void MANFST_print_entry(MANFST_entry_t *entry);

int MANFST_entry_list_type(SYNTAX_pnode_t *head)
{
	assert(head);
	assert(head->type == SYNTAX_PNODE_LIST);

	bool passed = true;
	assert(head->size > 0);
	tok_type_t first = head->nodes[0]->token->type;

	if(head->size > 1)
	{
		for(uint64_t i = 1; i < head->size; ++i)
		{
			SYNTAX_pnode_t *node = head->nodes[i];

			if(node->token->type != first)
			{
				passed = false;
			}
		}
	}

	if(passed == false)
		return -1;

	return (int)first;


}

void MANFST_append_entry_prototag(MANFST_entry_t *entry, MANFST_prototag_t *tag, uint64_t scope)
{
	if(entry->entryscope_alloc <= entry->entryscope_size)
	{
		entry->entryscope_alloc = (entry->entryscope_size * 2) + 1;
		entry->entryscope = realloc_safe(entry->entryscope, entry->entryscope_alloc, sizeof(MANFST_entryscope_t));
	}
	MANFST_entryscope_t entryscope =
	{
		.prototag = tag,
		.scope = scope,
	};

	entry->entryscope[entry->entryscope_size++] = entryscope;
}

void MANFST_init_entry_mono(MANFST_entry_t *entry, SYNTAX_pnode_t *head, char *key, char *name, uint64_t namespace)
{

	SYNTAX_pnode_t *value = head->nodes[0];
	tok_t *n1_tok = value->token;

	MANFST_prototag_t *prototag = NULL;
	if(n1_tok->type == TOK_INT)
	{
		int64_t integer = atoi(n1_tok->token);
		prototag = MANFST_init_prototag_int(key, name, integer);
	}
	else if(n1_tok->type == TOK_STRING)
	{
		prototag = MANFST_init_prototag_string(key, name, n1_tok->token);
	}
	else if(n1_tok->type == TOK_WORD)
	{
		int test = str_to_bool(n1_tok->token);
		if(test == -1)
		{
			fprintf(stderr, "unrecognized word %s\n", n1_tok->token);
			exit(EXIT_FAILURE);
			assert(0);
		}
		prototag = MANFST_init_prototag_bool(key, name, test);
	}
	else
	{
		fprintf(stderr, "TOKEN TYPE NOT KNOWN %s:%d\n", n1_tok->token, n1_tok->type);
		exit(EXIT_FAILURE);
		assert(0);
	}
	assert(prototag != NULL && "cannot fail to init a prototag");

	MANFST_append_entry_prototag(entry, prototag, 0);
}

void MANFST_init_entry_list(MANFST_entry_t *entry, SYNTAX_pnode_t *head, char *key, char *name,uint64_t namespace)
{
	int test_type = MANFST_entry_list_type(head);
	assert(test_type != -1 && "all types of a list did not match");
	tok_type_t type = (tok_type_t)test_type;


	const uint64_t size = head->size;
	uint64_t buffer[size];
	memset(buffer, 0, size * sizeof(uint64_t));
	MANFST_prototag_t *prototag = NULL;
	if(type == TOK_INT)
	{
		int64_t *ptr = (int64_t *)buffer;

		for(uint64_t i = 0; i < size; ++i)
		{
			SYNTAX_pnode_t *node = head->nodes[i];
			int64_t tmp = atoi(node->token->token);
			ptr[i] = tmp;
		}

		prototag = MANFST_init_prototag_list_ints(key, name, ptr, size);
	}
	else if(type == TOK_STRING)
	{
		char **ptr = (char **)buffer;


		for(uint64_t i = 0; i < size; ++i)
		{
			SYNTAX_pnode_t *node = head->nodes[i];
			ptr[i] = node->token->token;
		}
		prototag = MANFST_init_prototag_list_string(key,  name,ptr, size);

	}
	else if(type == TOK_WORD)
	{

		bool *ptr = (bool *)buffer;
		for(uint64_t i = 0; i < size; ++i)
		{
			SYNTAX_pnode_t *node = head->nodes[i];
			int test = str_to_bool(node->token->token);
			if(test == -1)
			{
				fprintf(stderr, "unrecognized word %s\n", node->token->token);
				exit(EXIT_FAILURE);
				assert(0);
			}
			ptr[i] = (bool)test;
		}
		prototag = MANFST_init_prototag_list_bools(key, name, ptr, size);
	}
	else
	{
		assert(0 && "Inconsistent with the valid TAG types");
	}
	assert(prototag != NULL);
	MANFST_append_entry_prototag(entry, prototag, 0);
}

void MANFST_init_entry_map(MANFST_entry_t *entry, SYNTAX_pnode_t *head, char *key, char *name,uint64_t namespace)
{
	assert(0);
}

/*
//this is genuinely trash code, replace this
CFG_prototag_t *CFG_init_entry_tag(SYNTAX_pnode_t *head)
{

	assert(head);
	assert(head->nodes);
	assert(head->size == 2);
	char *key = head->nodes[0]->token->token;
	SYNTAX_pnode_t *n1 = head->nodes[1];

	if(n1->type == SYNTAX_PNODE_VALUE)
	{
		tok_t *n1_tok = n1->token;
		CFG_prototag_t *prototag = NULL;
		if(n1_tok->type == TOK_INT)
		{
			int64_t integer = atoi(n1_tok->token);

			prototag = CFG_init_prototag_int(key, integer);
		}
		else if(n1_tok->type == TOK_STRING)
		{
			prototag = CFG_init_prototag_string(key, n1_tok->token);
		}
		else if(n1_tok->type == TOK_WORD)
		{

			int test = str_to_bool(n1_tok->token);
			if(test == -1)
			{
				fprintf(stderr, "unrecognized word %s\n", n1_tok->token);
				exit(EXIT_FAILURE);
				assert(0);
			}
			prototag = CFG_init_prototag_bool(key, test);

		}
		else
		{
			fprintf(stderr, "TOKEN TYPE NOT KNOWN %s:%d\n", n1_tok->token, n1_tok->type);
			exit(EXIT_FAILURE);
			assert(0);

		}
		assert(prototag != NULL && "cannot fail to init a prototag");

		return prototag;
	}
	else if(n1->type == SYNTAX_PNODE_LIST)
	{


	}
	else
	{
		assert(0);
		return NULL;
	}



}

*/
MANFST_entry_t *MANFST_init_entry(SYNTAX_pnode_t *start, char *class)
{
	MANFST_entry_t *entry = calloc(1, sizeof(MANFST_entry_t));
	assert(entry);

	entry->class= strdup(class);
	assert(entry->class);


	entry->entryscope_size = 0;
	entry->entryscope_alloc = start->size;
	MANFST_entryscope_t *list =  calloc(entry->entryscope_alloc, sizeof(MANFST_entryscope_t ));
	entry->entryscope = list;

	for(uint64_t i = 0; i < start->size; ++i)
	{
		assert(0 && "build ast");
		//list[i] = CFG_init_entry_tag(start->nodes[i]);
	}


	return entry;
}



void MANFST_free_entry(MANFST_entry_t *entry)
{
	free(entry->class);
	free(entry);

}
void MANFST_print_entry(MANFST_entry_t *entry)
{
	printf("entry: %s\n", entry->class);

	for(uint64_t i = 0; i < entry->entryscope_size; ++i)
	{
		printf("scope: %lu ", entry->entryscope[i].scope);
		MANFST_print_prototag(entry->entryscope[i].prototag);
	}

}