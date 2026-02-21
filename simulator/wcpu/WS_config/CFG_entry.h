#ifndef __WS_CFG_ENTRY_HEADER__
#define __WS_CFG_ENTRY_HEADER__

#include "CFG_prototag.h"
#include "IO_parser.h"
#include "TAG_tag.h"

#include <stdint.h>


//this is temporary, and very britle. we can work towards a new config system... on rework
typedef struct CFG_entry
{
	char *class;
	uint64_t prototags_size;
	uint64_t prototags_alloc;
	CFG_prototag_t **prototags;
	uint64_t namespaces_size;
	uint64_t *namespaces;


}CFG_entry_t;

static CFG_entry_t *CFG_alloc_entry(char *class);

void CFG_append_entry_prototag(CFG_entry_t *entry, CFG_prototag_t *tag);

void CFG_init_entry_mono(CFG_entry_t *entry, IO_pnode_t *head, char *key, char *name, uint64_t namespace);
void CFG_init_entry_list(CFG_entry_t *entry, IO_pnode_t *head, char *key, char *name,uint64_t namespace);
void CFG_init_entry_map(CFG_entry_t *entry, IO_pnode_t *head, char *key, char *name,uint64_t namespace);
//returns TAG_type_t or -1 if not all the same
int CFG_entry_list_type(IO_pnode_t *head);

CFG_prototag_t *CFG_init_entry_tag(IO_pnode_t *head);
CFG_entry_t *CFG_init_entry(IO_pnode_t *start, char *class);

void CFG_free_entry(CFG_entry_t *entry);
void CFG_print_entry(CFG_entry_t *entry);

#endif