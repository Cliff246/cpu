#include "MANFST_manifest.h"
#include "MANFST_tag.h"
#include "SYNTAX_parser.h"
#include "TAG_list.h"
#include "TAG_map.h"
#include "TAG_tag.h"
#include "token.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*

uint64_t MANFST_size_manifest(SYNTAX_ptree_t *tree)
{
	uint64_t count = 0;
	for(uint64_t i = 0; i < tree->head->size; ++i)
	{
		SYNTAX_pnode_t *entry = tree->head->nodes[i];
		count += entry->size;

	}
	return count;

}

MANFST_prototag_t *MANFST_init_entry_tag(SYNTAX_pnode_t *head);

void MANFST_free_entry(MANFST_entry_t *entry);
void MANFST_print_entry(MANFST_entry_t *entry);



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
*/

TAG_tag_t *MANFST_init_entry_mono(SYNTAX_pnode_t *head)
{
	TAG_tag_t *tag = NULL;
	tok_t *token = head->token;
	if(token->type == TOK_INT)
	{
		int64_t integer = atoi(token->token);
		tag = MANFST_init_tag_int(integer);
	}
	else if(token->type == TOK_STRING)
	{
		tag = MANFST_init_tag_string(token->token);
	}
	else if(token->type == TOK_WORD)
	{
		int test = str_to_bool(token->token);
		if(test == -1)
		{
			fprintf(stderr, "unrecognized word %s\n", token->token);
			exit(EXIT_FAILURE);
			assert(0);
		}
		tag = MANFST_init_tag_bool(test);
	}
	else
	{
		fprintf(stderr, "TOKEN TYPE NOT KNOWN %s:%d\n", token->token, token->type);
		exit(EXIT_FAILURE);
		assert(0);
	}
	assert(tag!= NULL && "cannot fail to init a prototag");

	return tag;
}




TAG_tag_t *MANFST_init_entry_map(SYNTAX_pnode_t *head)
{
	//printf("\n\n");
	TAG_tag_t *tag = NULL;
	SYNTAX_pnode_t *third = head->nodes[2];
	if(third->type == SYNTAX_PNODE_MONO)
	{
		tag = MANFST_init_entry_mono(third->nodes[0]);

	}
	else if(third->type == SYNTAX_PNODE_LIST)
	{
		tag = MANFST_init_tag_list();
		TAG_argptr_t argptr = TAG_get_fn(TAG_LIST, TAG_FN_LIST_APPEND);


		for(uint64_t i = 0; i < third->size; ++i)
		{
			SYNTAX_pnode_t *cur = third->nodes[i];

			TAG_tag_t *tmp = MANFST_init_entry_mono(cur);
			argptr.LIST->append(tag, tmp);
		}
	}
	else
	{
		tag = MANFST_init_tag_map();
		TAG_argptr_t set_key_str_argptr = TAG_get_fn(TAG_MAP, TAG_FN_MAP_SET_KEY_STRING);
		TAG_argptr_t get_key_str_argptr = TAG_get_fn(TAG_MAP, TAG_FN_MAP_GET_KEY_STRING);
		TAG_argptr_t set_key_int_argptr = TAG_get_fn(TAG_MAP, TAG_FN_MAP_SET_KEY_INT);

		for(uint64_t j = 0; j < third->size; ++j)
		{
			SYNTAX_pnode_t *cur = third->nodes[j];
			char *key =  cur->nodes[0]->token->token;
			TAG_tag_t *tmp = get_key_str_argptr.MAP->get_key_string(tag, key);
			if(tmp == NULL)
			{
				TAG_tag_t *map = MANFST_init_tag_map();
				//printf("set key string %s\n", key);
				set_key_str_argptr.MAP->set_key_string(tag, key, map);
			}
		}
		for(uint64_t i = 0; i < third->size; ++i)
		{

			SYNTAX_pnode_t *cur = third->nodes[i];
			char *key =  cur->nodes[0]->token->token;

			TAG_tag_t *submap = get_key_str_argptr.MAP->get_key_string(tag, key);
			assert(submap);
			TAG_tag_t *tmp =  MANFST_init_entry_map(cur);

			//printf("map: %s\n", cur->nodes[0]->token->token);

			if(cur->nodes[1]->type == SYNTAX_PNODE_NONAME)
			{
				int64_t id = (((int64_t)rand()) << 32) | (int64_t)rand();

				//printf("%ld\n", id);
				set_key_int_argptr.MAP->set_key_int(submap, id, tmp);
			}
			else
			{
				set_key_str_argptr.MAP->set_key_string(submap, cur->nodes[1]->token->token, tmp);

			}





		}

	}
	TAG_print(tag);
	return tag;
}

MANFST_manifest_t *MANFST_init_manifest(SYNTAX_ptree_t *tree)
{

	MANFST_manifest_t *manifest = calloc(1, sizeof(MANFST_manifest_t));
	manifest->head = MANFST_init_entry_map( tree->head->nodes[0]);


	/*
	assert(tree);
	assert(tree->head != NULL);
	CFG_manifest_t *manifest = calloc(1, sizeof(CFG_manifest_t));
	assert(manifest);
	uint64_t size = CFG_size_manifest(tree);
	CFG_entry_t **entries = calloc(size, sizeof(CFG_entry_t *));
	assert(entries);
	uint64_t current = 0;
	for(uint64_t i = 0; i < tree->head->size; ++i)
	{
		SYNTAX_pnode_t *scope = tree->head->nodes[i];
		char *module = scope->token->token;
		//printf("%s\n", module);
		for(uint64_t k = 0; k < scope->size; ++k)
		{
			//printf("\n\n");
			SYNTAX_pnode_t *local = scope->nodes[k];
			assert(local);
			CFG_entry_t *entry = CFG_init_entry(local, module);
			//CFG_print_entry(entry);
			entries[current++] = entry;

		}

	}
	manifest->size = size;
	manifest->entries = entries;

	const uint64_t settings_size = tree->settings->size;
	assert(settings_size > 0 && "must have one setting(ie one wire)");
	CFG_setting_t **settings = calloc(settings_size, sizeof(CFG_setting_t *));
	assert(settings);
	for(uint64_t j = 0; j < settings_size; ++j)
	{
		SYNTAX_pnode_t *nodes = tree->settings->nodes[j];
		CFG_setting_t *set = CFG_init_setting(nodes);
		assert(set);
		settings[j] = set;
	}
	manifest->settings = settings;
	manifest->settings_size = settings_size;

	return manifest;
	*/
}
void MANFST_free_manifest(MANFST_manifest_t *manifest)
{
	//free(manifest->entries);

	//for(uint64_t i = 0; i <manifest->settings_size; ++i)
	//{
	//	MANFST_free_setting(manifest->settings[i]);
	//}
	//free(manifest->settings);
	//free(manifest);
}