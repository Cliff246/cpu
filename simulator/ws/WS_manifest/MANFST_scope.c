#include "MANFST_scope.h"
#include "MANFST_tag.h"
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

TAG_tag_t *MANFST_init_scope_mono(SYNTAX_pnode_t *head)
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



TAG_tag_t *MANFST_init_scope_list(SYNTAX_pnode_t *head)
{
	SYNTAX_pnode_t *third = head->nodes[2];
	
	
	TAG_argptr_t argptr = TAG_get_fn(TAG_LIST, TAG_FN_LIST_APPEND);	

	TAG_tag_t *tag = MANFST_init_tag_list();
	for(uint64_t i = 0; i < third->size; ++i)
	{

		SYNTAX_pnode_t *element = third->nodes[i];
			
			
		TAG_tag_t *tag_element;			
		
		if(element->type == SYNTAX_PNODE_LIST)
		{
			SYNTAX_pnode_t *list_start = element->nodes[0];
			tag_element = MANFST_init_scope_list(list_start);
		}
		else if(element->type == SYNTAX_PNODE_MAP)
		{
			SYNTAX_pnode_t *map_start = element->nodes[0];
			tag_element = MANFST_init_scope_map(map_start);
		}
		else
		{
			tag_element = MANFST_init_scope_mono(element);
		}

		argptr.LIST->append(tag, tag_element);
	}	
	return tag;
}

TAG_tag_t *MANFST_init_scope_map(SYNTAX_pnode_t *head)
{
	TAG_tag_t *tag = NULL;
	SYNTAX_pnode_t *third = head->nodes[2];
	if(third->type == SYNTAX_PNODE_MONO)
	{
		tag = MANFST_init_scope_mono(third->nodes[0]);

	}
	else if(third->type == SYNTAX_PNODE_LIST)
	{
		
		tag = MANFST_init_scope_list(head);
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
			TAG_tag_t *tmp =  MANFST_init_scope_map(cur);

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
	return tag;
}

void MANFST_init_scope(MANFST_scope_t *scope, SYNTAX_pnode_t *head)
{
	scope->scope = MANFST_init_scope_map(head);
	scope->code = strdup(head->nodes[0]->token->token);

}

void MANFST_print_scope(MANFST_scope_t *scope)
{
	printf("scope: %s\n", scope->code);
	TAG_print(scope->scope);
}

void MANFST_free_scope(MANFST_scope_t *scope)
{
	//printf("free \n");
	//TAG_print(scope->scope);
	TAG_free(scope->scope);
	free(scope->code);
}
