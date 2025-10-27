#include "scope.h"

#include "parser.h"
#include "reference.h"
#include "entry.h"

#include <stdlib.h>
#include <stdio.h>
#include "commons.h"
#include "symbol.h"


void add_entry_to_scope(scope_t *scope, entry_t *entry)
{
	printf("%d\n", entry->bytes);




	if(scope->entries.alloc < 100)
		scope->entries.entries = CALLOC((scope->entries.alloc = 100), entry_t *);
	else if(scope->entries.alloc <= scope->entries.count)
		scope->entries.entries = REALLOC(scope->entries.entries, (scope->entries.alloc *= 2), entry_t *);

	if(entry->type == ENTRY_MOP)
	{
		if(entry->entry.mop.type == MOP_TYPE_DEFINE_CONFIG)
		{
			if(entry->entry.mop.holder.config.type == MOP_CONFIG_TYPE_PUBLIC)
			{
				ref_t *ref = create_reference(entry->entry.mop.holder.config.config.pub.key_string);
				ref->access_modifier = ACCESS_PUBLIC;
				symbol_t *sym = create_symbol_with_ref(ref);
				add_symbol_to_scope(scope, sym);


			}
			else if(entry->entry.mop.holder.config.type == MOP_CONFIG_TYPE_INCLUDE)
			{
				ref_t *ref = create_reference(entry->entry.mop.holder.config.config.inc.file_include);
				symbol_t *sym = create_symbol_with_ref(ref);

				add_symbol_to_scope(scope, sym);
			}

			else if(entry->entry.mop.holder.config.type == MOP_CONFIG_TYPE_DEFINE)
			{
				ref_t *ref = create_reference(entry->entry.mop.holder.config.config.def.key_imply);
				symbol_t *sym = create_symbol_with_ref(ref);

				add_symbol_to_scope(scope, sym);


			}
		}

	}
		scope->bytes += entry->bytes;

	scope->entries.entries[scope->entries.count++] = entry;

}


void add_symbol_to_scope(scope_t *scope, symbol_t *sym)
{
	//print_ref(ref);
	printf("add reference\n");
	if(scope->symbols.alloc < 100)
	{

		scope->symbols.symbols = CALLOC((scope->symbols.alloc = 100), symbol_t *);
	}
	else if(scope->symbols.alloc <= scope->symbols.count)
	{
		scope->symbols.symbols = REALLOC(scope->symbols.symbols, (scope->symbols.alloc *= 2), symbol_t *);

	}
	scope->symbols.symbols[scope->symbols.count++] = sym;
}

void create_ref_from_parser(scope_t *scope, parse_node_t *head)
{
	ref_t *ref = create_reference(head->tok->lexeme);


	//creates the references
	implement_reference(ref, head);

	//set sid
	ref->sid = scope->segment.sid;
	ref->locale_offset = (scope->bytes / 4);
	printf("scope bytes: %d\n", scope->bytes / 4);
	print_ref(ref);


	for(int i = 0; i < head->child_count; ++i)
	{
		parse_node_t *child = head->children[i];
		//could be stupid idk
		if(child->kind == NODE_METAOP || child->kind == NODE_INSTR)
		{
			entry_t *entry = create_entry(child);
			add_entry_to_scope(scope, entry);

		}
	}
	symbol_t *sym = create_symbol_with_ref(ref);
	add_symbol_to_scope(scope, sym);
}


scope_t create_scope(parse_node_t *head)
{
	scope_t scope = {0};
	scope.segment = create_segment(head);

	for(int i = 0; i < head->child_count; ++i)
	{
		parse_node_t *child = head->children[i];
		//printf("b%d\n", scope.bytes);
		//print_depth(child, 0);

		if(child->kind == NODE_REFERENCE)
		{
			create_ref_from_parser(&scope, child);
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