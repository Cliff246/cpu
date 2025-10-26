#ifndef __SCOPE_HEADER__
#define __SCOPE_HEADER__

#include <stdlib.h>
#include <stdint.h>


#include "entry.h"
#include "segment.h"
#include "reference.h"




typedef struct scope_entries
{
	entry_t **entries;

	size_t count;
	size_t alloc;

}scope_entries_t;

typedef struct scope_refs
{
	ref_t **refs;
	size_t count;
	size_t alloc;
}scope_refs_t;


typedef struct scope
{
	scope_entries_t entries;
	scope_refs_t refs;

	seg_t segment;

	parse_node_t *head;
}scope_t;


scope_t create_scope(parse_node_t *head);


#endif