#ifndef __MANFST_MANIFEST_HEADER__
#define __MANFST_MANIFEST_HEADER__

#include "SYNTAX_parser.h"
#include "MANFST_scope.h"
#include "TAG_tag.h"
#include <stdint.h>


typedef struct MANFST_manifest
{
	uint64_t scopes_size;
	MANFST_scope_t *scopes;
	SYNTAX_ptree_t *tree;
}MANFST_manifest_t;



uint64_t MANFST_size_manifest(SYNTAX_ptree_t *tree);
MANFST_manifest_t *MANFST_init_manifest(SYNTAX_ptree_t *tree);
void MANFST_free_manifest(MANFST_manifest_t *manifest);


#endif
