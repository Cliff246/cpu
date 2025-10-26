#include "scope.h"

#include "parser.h"
#include "reference.h"
#include "entry.h"

#include <stdlib.h>
#include <stdio.h>
#include "commons.h"

void add_entry_to_scope(scope_t *scope, entry_t *entry)
{

	if(scope->entries.alloc < 100)
	{

		scope->entries.entries = CALLOC((scope->entries.alloc = 100), entry_t *);
	}
	else if(scope->entries.alloc <= scope->entries.count)
	{
		scope->entries.entries = REALLOC(scope->entries.entries, (scope->entries.alloc *= 2), entry_t *);

	}
	scope->entries.entries[scope->entries.count++] = entry;
}


void add_reference_to_scope(scope_t *scope, parse_node_t *head)
{
	ref_t *ref = create_reference(head->tok->lexeme);


	//creates the references
	implement_reference(ref, head);


	//set sid
	ref->sid = scope->segment.sid;

	if(scope->refs.alloc < 100)
	{

		scope->refs.refs = CALLOC((scope->refs.alloc = 100), ref_t *);
	}
	else if(scope->refs.alloc <= scope->refs.count)
	{
		scope->refs.refs = REALLOC(scope->refs.refs, (scope->refs.alloc *= 2), ref_t *);

	}
	scope->refs.refs[scope->refs.count++] = ref;

	for(int i = 0; i < head->child_count; ++i)
	{
		parse_node_t *child = head->children[i];
		//could be stupid idk
		if(head->kind == NODE_METAOP || head->kind == NODE_INSTR)
		{
			entry_t *entry = create_entry(child);
			add_entry_to_scope(scope, entry);

		}
	}

}


scope_t create_scope(parse_node_t *head)
{
	scope_t scope = {0};
	scope.segment = create_segment(head);

	for(int i = 0; i < head->child_count; ++i)
	{
		parse_node_t *child = head->children[i];
		//print_depth(child, 0);

		if(child->kind == NODE_REFERENCE)
		{
			add_reference_to_scope(&scope, child);
		}
		else if(child->kind == NODE_METAOP)
		{
			entry_t *mop_entry = create_entry(child);
			printf("mop entry\n");

			add_entry_to_scope(&scope, mop_entry);

		}
		else if(child->kind == NODE_ARGS)
		{

		}
		else if(child->kind == NODE_CHILDS)
		{

		}

	}
	return scope;
}