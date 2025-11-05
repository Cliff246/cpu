#include "scope.h"

#include "parser.h"
#include "reference.h"
#include "entry.h"

#include <stdlib.h>
#include <stdio.h>
#include "commons.h"
#include "symbol.h"
#include "fileio.h"

void print_scope_symbols(scope_t *scope)
{
	printf("scope sid:%d tag:%d file:%s:\n", scope->segment.sid, scope->segment.tag, get_filename_from_id(get_scope_file_id(scope)));
	for(int i = 0; i < scope->symbols.count; ++i)
	{
		symbol_t *sym = scope->symbols.symbols[i];
		printf("\t");
		print_symbol(sym);
	}
}

int get_scope_file_id(scope_t *scope)
{
	return scope->segment.fid;
}

void add_entry_to_scope(scope_t *scope, entry_t *entry)
{
	//printf("%d\n", entry->bytes);




	if(scope->entries.alloc == 0)
		scope->entries.entries = CALLOC((scope->entries.alloc = 100), entry_t *);
	else if(scope->entries.alloc <= scope->entries.count)
		scope->entries.entries = REALLOC(scope->entries.entries, (scope->entries.alloc *= 2), entry_t *);

	if(scope->entries.consumed_alloc == 0)
	{
		scope->entries.consumed = CALLOC((scope->entries.consumed_alloc = 10), char *);
	}
	else if(scope->entries.consumed_alloc <= scope->entries.consumed_count)
	{
		scope->entries.consumed = REALLOC(scope->entries.consumed, (scope->entries.consumed_alloc *= 2), char *);
	}
	if(entry->entry.inst.immref != NULL)
	{
		//valid until instruction goes out of scope which should be together
		scope->entries.consumed[scope->entries.consumed_count++] = entry->entry.inst.immref;
	}
	scope->bytes += entry->bytes;

	scope->entries.entries[scope->entries.count++] = entry;

}


void add_symbol_to_scope(scope_t *scope, symbol_t *sym)
{
	//print_ref(ref);
	//printf("add reference\n");
	if(scope->symbols.alloc == 0)
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
	ref->locale_offset = scope->entries.count;
	ref->byte_offset = scope->bytes;
	//printf("scope bytes: %d\n", scope->bytes / 4);
	//print_ref(ref);
	parse_node_t *entry_head = get_entries_under_reference(ref);

	for(int i = 0; i < entry_head->child_count; ++i)
	{
		parse_node_t *child = entry_head->children[i];
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
	if(head->child_count != 2)
	{
		printf("scope should have 2 children had %d\n", head->child_count);
		exit(1);
	}

	parse_node_t *arg_node = head->children[0];
	parse_node_t *seg_node = head->children[1];

	scope.segment = create_segment(arg_node);


	for(int i = 0; i < seg_node->child_count; ++i)
	{
		parse_node_t *child = seg_node->children[i];
		//printf("b%d\n", scope.bytes);
		//print_depth(child, 0);
		if(child->kind == NODE_REFERENCE)
		{
			create_ref_from_parser(&scope, child);
		}
		else if(child->kind == NODE_METAOP)
		{
			entry_t *mop_entry = create_entry(child);
			//printf("mop entry\n");

			add_entry_to_scope(&scope, mop_entry);

		}
	}
	return scope;
}

bool scope_uses_symbol(scope_t *scope, char *key)
{
	if(scope->entries.consumed_count > 0)
	{
		for(int i = 0; i < scope->entries.consumed_count; ++i)
		{
			if(!strcmp(scope->entries.consumed[i], key))
				return true;

		}
		return false;
	}
	return false;
}

size_t get_scope_size(scope_t *scope)
{
	return scope->entries.count;
}