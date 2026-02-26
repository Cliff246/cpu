#include "MANFST_manifest.h"
#include "MANFST_tag.h"
#include "SYNTAX_parser.h"
#include "TAG_list.h"
#include "TAG_map.h"
#include "TAG_tag.h"
#include "token.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

MANFST_manifest_t *MANFST_init_manifest(SYNTAX_ptree_t *tree)
{

	MANFST_manifest_t *manifest = calloc(1, sizeof(MANFST_manifest_t));
	SYNTAX_pnode_print(tree->head, 0);

	uint64_t scopes_size= tree->head->size;
	
	MANFST_scope_t *scopes = calloc(scopes_size, sizeof(MANFST_scope_t));
	
	manifest->scopes = scopes;
	manifest->scopes_size = scopes_size;

	for(uint64_t i = 0; i < scopes_size; ++i)
	{
		MANFST_init_scope(&manifest->scopes[i], tree->head->nodes[i]);	
	}



	return manifest;
}
void MANFST_free_manifest(MANFST_manifest_t *manifest)
{
	//free(manifest->entries);

	//for(uint64_t i = 0; i <manifest->settings_size; ++i)
	//{
	//	MANFST_free_setting(manifest->settings[i]);
	//}
	//free(manifest->settings);
	//free(manifest);
}
