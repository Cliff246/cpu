#ifndef __MANFST_ENTRY_HEADER__
#define __MANFST_ENTRY_HEADER__

#include "MANFST_prototag.h"
#include "SYNTAX_parser.h"
#include "TAG_tag.h"

#include <stdint.h>
#include <sys/types.h>

typedef struct MANFST_entryscope
{
	MANFST_prototag_t *prototag;
	uint64_t scope;
}MANFST_entryscope_t;

//this is temporary, and very britle. we can work towards a new config system... on rework
typedef struct MANFST_entry
{
	char *class;

	uint64_t entryscope_size;
	uint64_t entryscope_alloc;
	MANFST_entryscope_t *entryscope;


}MANFST_entry_t;


void MANFST_append_entry_prototag(MANFST_entry_t *entry, MANFST_prototag_t *tag, uint64_t scope);
void MANFST_init_entry_mono(MANFST_entry_t *entry, SYNTAX_pnode_t *head, char *key, char *name, uint64_t namespace);
void MANFST_init_entry_list(MANFST_entry_t *entry, SYNTAX_pnode_t *head, char *key, char *name,uint64_t namespace);
void MANFST_init_entry_map(MANFST_entry_t *entry, SYNTAX_pnode_t *head, char *key, char *name,uint64_t namespace);
//returns TAG_type_t or -1 if not all the same
int MANFST_entry_list_type(SYNTAX_pnode_t *head);

MANFST_prototag_t *MANFST_init_entry_tag(SYNTAX_pnode_t *head);
MANFST_entry_t *MANFST_init_entry(SYNTAX_pnode_t *start, char *class);

void MANFST_free_entry(MANFST_entry_t *entry);
void MANFST_print_entry(MANFST_entry_t *entry);

#endif
