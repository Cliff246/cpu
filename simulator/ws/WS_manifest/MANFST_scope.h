#ifndef __MANFST_SCOPE_HEADER__
#define __MANFST_SCOPE_HEADER__


#include "TAG_tag.h"
#include "SYNTAX_parser.h"
#include <stdint.h>

typedef struct MANFST_scope
{
	TAG_tag_t *scope;	
	char *code;
}MANFST_scope_t;

void MANFST_init_scope(MANFST_scope_t *scope, SYNTAX_pnode_t *head); 


#endif
