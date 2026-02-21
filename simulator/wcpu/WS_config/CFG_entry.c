#include "CFG_entry.h"
#include "CFG_prototag.h"
#include "IO_parser.h"
#include "TAG_tag.h"
#include "commons.h"
#include "token.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static CFG_entry_t *CFG_alloc_entry(char *class)
{
	assert(class);
	CFG_entry_t *ptr = calloc(1, sizeof(CFG_entry_t));
	assert(ptr);

	ptr->class= strdup(class);
	assert(ptr->class);

	ptr->namespaces_size = 0;
	ptr->prototags = NULL;
	return ptr;
}

int CFG_entry_list_type(IO_pnode_t *head)
{
	assert(head);
	assert(head->type == IO_PNODE_LIST);

	bool passed = true;
	assert(head->size > 0);
	tok_type_t first = head->nodes[0]->token->type;

	if(head->size > 1)
	{
		for(uint64_t i = 1; i < head->size; ++i)
		{
			IO_pnode_t *node = head->nodes[i];

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

void CFG_append_entry_prototag(CFG_entry_t *entry, CFG_prototag_t *tag)
{
	if(entry->prototags_alloc <= entry->prototags_size)
	{
		entry->prototags_alloc = (entry->prototags_alloc * 2) + 1;
		entry->prototags = realloc_safe(entry->prototags, entry->prototags_alloc, sizeof(CFG_prototag_t *));
	}
	entry->prototags[entry->prototags_size++] = tag;
}

void CFG_init_entry_mono(CFG_entry_t *entry, IO_pnode_t *head, char *key, char *name,  uint64_t namespace)
{

	IO_pnode_t *value = head->nodes[0];
	tok_t *n1_tok = value->token;

	CFG_prototag_t *prototag = NULL;
	if(n1_tok->type == TOK_INT)
	{
		int64_t integer = atoi(n1_tok->token);
		prototag = CFG_init_prototag_int(key, name, namespace, integer);
	}
	else if(n1_tok->type == TOK_STRING)
	{
		prototag = CFG_init_prototag_string(key, name, namespace, n1_tok->token);
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
		prototag = CFG_init_prototag_bool(key, name, namespace, test);
	}
	else
	{
		fprintf(stderr, "TOKEN TYPE NOT KNOWN %s:%d\n", n1_tok->token, n1_tok->type);
		exit(EXIT_FAILURE);
		assert(0);
	}
	assert(prototag != NULL && "cannot fail to init a prototag");

	CFG_append_entry_prototag(entry, prototag);
}
void CFG_init_entry_list(CFG_entry_t *entry, IO_pnode_t *head, char *key, char *name, uint64_t namespace)
{
	int test_type = CFG_entry_list_type(head);
	assert(test_type != -1 && "all types of a list did not match");
	tok_type_t type = (tok_type_t)test_type;


	const uint64_t size = head->size;
	uint64_t buffer[size];
	memset(buffer, 0, size * sizeof(uint64_t));
	CFG_prototag_t *prototag = NULL;
	if(type == TOK_INT)
	{
		int64_t *ptr = (int64_t *)buffer;

		for(uint64_t i = 0; i < size; ++i)
		{
			IO_pnode_t *node = head->nodes[i];
			int64_t tmp = atoi(node->token->token);
			ptr[i] = tmp;
		}

		prototag = CFG_init_prototag_list_ints(key, name, namespace, ptr, size);
	}
	else if(type == TOK_STRING)
	{
		char **ptr = (char **)buffer;


		for(uint64_t i = 0; i < size; ++i)
		{
			IO_pnode_t *node = head->nodes[i];
			ptr[i] = node->token->token;
		}
		prototag = CFG_init_prototag_list_string(key,  name, namespace,ptr, size);

	}
	else if(type == TOK_WORD)
	{

		bool *ptr = (bool *)buffer;
		for(uint64_t i = 0; i < size; ++i)
		{
			IO_pnode_t *node = head->nodes[i];
			int test = str_to_bool(node->token->token);
			if(test == -1)
			{
				fprintf(stderr, "unrecognized word %s\n", node->token->token);
				exit(EXIT_FAILURE);
				assert(0);
			}
			ptr[i] = (bool)test;
		}
		prototag = CFG_init_prototag_list_bools(key, name, namespace, ptr, size);
	}
	else
	{
		assert(0 && "Inconsistent with the valid TAG types");
	}
	assert(prototag != NULL);
	CFG_append_entry_prototag(entry, prototag);
}
void CFG_init_entry_map(CFG_entry_t *entry, IO_pnode_t *head, char *key, char *name, uint64_t namespace)
{
	assert(0);
}

/*
//this is genuinely trash code, replace this
CFG_prototag_t *CFG_init_entry_tag(IO_pnode_t *head)
{

	assert(head);
	assert(head->nodes);
	assert(head->size == 2);
	char *key = head->nodes[0]->token->token;
	IO_pnode_t *n1 = head->nodes[1];

	if(n1->type == IO_PNODE_VALUE)
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
	else if(n1->type == IO_PNODE_LIST)
	{


	}
	else
	{
		assert(0);
		return NULL;
	}



}

*/

CFG_entry_t *CFG_init_entry(IO_pnode_t *start, char *class)
{
	CFG_entry_t *entry =  CFG_alloc_entry(class);
	assert(entry);

	entry->prototags_size = 0;
	entry->prototags_alloc = start->size;
	CFG_prototag_t **list =  calloc(entry->prototags_alloc, sizeof(CFG_prototag_t *));

	for(uint64_t i = 0; i < start->size; ++i)
	{
		assert(0 && "build ast");
		//list[i] = CFG_init_entry_tag(start->nodes[i]);
	}

	entry->prototags = list;
	entry->prototags_size = start->size;

	return entry;
}

void CFG_free_entry(CFG_entry_t *entry)
{
	free(entry->class);
	free(entry);

}

void CFG_print_entry(CFG_entry_t *entry)
{
	printf("entry: %s\n", entry->class);

	for(uint64_t i = 0; i < entry->prototags_size; ++i)
	{
		CFG_print_prototag(entry->prototags[i]);
	}

}