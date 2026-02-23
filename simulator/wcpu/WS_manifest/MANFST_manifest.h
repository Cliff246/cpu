#ifndef __MANFST_MANIFEST_HEADER__
#define __MANFST_MANIFEST_HEADER__

#include "MANFST_entry.h"
#include "MANFST_setting.h"
#include "SYNTAX_parser.h"
#include <stdint.h>


typedef struct MANFST_manifest
{
	uint64_t entries_size;
	MANFST_entry_t **entries;

	uint64_t settings_size;
	MANFST_setting_t **settings;
	SYNTAX_ptree_t *tree;
}MANFST_manifest_t;



uint64_t MANFST_size_manifest(SYNTAX_ptree_t *tree);
MANFST_manifest_t *MANFST_init_manifest(SYNTAX_ptree_t *tree);
void MANFST_free_manifest(MANFST_manifest_t *manifest);


#endif