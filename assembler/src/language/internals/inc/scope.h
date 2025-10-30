#ifndef __SCOPE_HEADER__
#define __SCOPE_HEADER__

#include <stdlib.h>
#include <stdint.h>


#include "entry.h"
#include "segment.h"
#include "reference.h"
#include "symbol.h"



typedef struct scope_entries
{
	entry_t **entries;
	size_t count;
	size_t alloc;


	char **consumed;
	size_t consumed_count;
	size_t consumed_alloc;

}scope_entries_t;

typedef struct scope_symbols
{
	symbol_t **symbols;
	size_t count;
	size_t alloc;
}scope_symbols_t;


typedef struct scope
{
	scope_entries_t entries;
	scope_symbols_t symbols;
	seg_t segment;

	parse_node_t *head;
	size_t bytes;

}scope_t;
void create_ref_from_parser(scope_t *scope, parse_node_t *head);

void add_symbol_to_scope(scope_t *scope, symbol_t *ref);

scope_t create_scope(parse_node_t *head);
bool scope_uses_symbol(scope_t *scope, char *key);

#endif