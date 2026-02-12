#ifndef __WS_CFG_ENTRY_HEADER__
#define __WS_CFG_ENTRY_HEADER__

#include "CFG_prototag.h"
#include "IO_parser.h"
#include "TAG_tag.h"

#include <stdint.h>


//this is temporary, and very britle. we can work towards a new config system... on rework
typedef struct CFG_entry
{
	char *module;
	uint64_t size;
	CFG_prototag_t **prototags;
}CFG_entry_t;

static CFG_entry_t *CFG_alloc(char *module);


//returns TAG_type_t or -1 if not all the same
int CFG_entry_list_type(IO_pnode_t *head);

CFG_prototag_t *CFG_init_entry_tag(IO_pnode_t *head);
CFG_entry_t *CFG_init_entry(IO_pnode_t *start, char *module);

void CFG_free_entry(CFG_entry_t *entry);
void CFG_print_entry(CFG_entry_t *entry);

#endif