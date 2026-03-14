#include "MANFST_manifest.h"
#include "MANFST_scope.h"
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

uint64_t MANFST_get_size_manifest(MANFST_manifest_t *manifest)
{
	return manifest->scopes_size;
}

MANFST_manifest_t *MANFST_init_manifest(SYNTAX_ptree_t *tree)
{

	MANFST_manifest_t *manifest = calloc(1, sizeof(MANFST_manifest_t));
	//SYNTAX_pnode_print(tree->head, 0);

	uint64_t scopes_size= tree->head->size;

	MANFST_scope_t *scopes = calloc(scopes_size, sizeof(MANFST_scope_t));

	manifest->scopes = scopes;
	manifest->scopes_size = scopes_size;
	manifest->tree = tree;
	for(uint64_t i = 0; i < scopes_size; ++i)
	{
		MANFST_init_scope(&manifest->scopes[i], tree->head->nodes[i]);
	}



	return manifest;
}



void MANFST_free_manifest(MANFST_manifest_t *manifest)
{
	for(uint64_t i = 0; i <manifest->scopes_size; ++i)
	{
		MANFST_free_scope(&manifest->scopes[i]);
	}
	free(manifest->scopes);
	SYNTAX_ptree_free(manifest->tree);
	free(manifest);


}


void MANFST_print_manifest(MANFST_manifest_t *manifest)
{
	printf("manifest\n");
	uint64_t count = MANFST_get_size_manifest(manifest);
	for(uint64_t i = 0; i < count; ++i)
	{
		MANFST_print_scope(&manifest->scopes[i]);
	}
}