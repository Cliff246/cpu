#include "CFG_manifest.h"
#include "CFG_entry.h"
#include "CFG_setting.h"
#include "IO_parser.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>



uint64_t CFG_size_manifest(IO_ptree_t *tree)
{
	uint64_t count = 0;
	for(uint64_t i = 0; i < tree->head->size; ++i)
	{
		IO_pnode_t *entry =  tree->head->nodes[i];
		count += entry->size;

	}
	return count;

}

CFG_manifest_t *CFG_init_manifest(IO_ptree_t *tree)
{
	assert(tree);
	assert(tree->head != NULL);
	CFG_manifest_t *manifest = calloc(1, sizeof(CFG_manifest_t));
	assert(manifest);
	uint64_t size = CFG_size_manifest(tree);
	CFG_entry_t **entries = calloc(size, sizeof(CFG_entry_t *));
	assert(entries);
	uint64_t current = 0;
	for(uint64_t i = 0; i < tree->head->size; ++i)
	{
		IO_pnode_t *scope = tree->head->nodes[i];
		char *module = scope->token->token;
		//printf("%s\n", module);
		for(uint64_t k = 0; k < scope->size; ++k)
		{
			//printf("\n\n");
			IO_pnode_t *local = scope->nodes[k];
			assert(local);
			CFG_entry_t *entry = CFG_init_entry(local, module);
			//CFG_print_entry(entry);
			entries[current++] = entry;

		}

	}
	manifest->size = size;
	manifest->entries = entries;

	const uint64_t settings_size = tree->settings->size;
	assert(settings_size > 0 && "must have one setting(ie one wire)");
	CFG_setting_t **settings = calloc(settings_size, sizeof(CFG_setting_t *));
	assert(settings);
	for(uint64_t j = 0; j < settings_size; ++j)
	{
		IO_pnode_t *nodes = tree->settings->nodes[j];
		CFG_setting_t *set = CFG_init_setting(nodes);
		assert(set);
		settings[j] = set;
	}
	manifest->settings = settings;
	manifest->settings_size = settings_size;

	return manifest;
}

void CFG_free_manifest(CFG_manifest_t *manifest)
{
	free(manifest->entries);

	for(uint64_t i = 0; i <manifest->settings_size; ++i)
	{
		CFG_free_setting(manifest->settings[i]);
	}
	free(manifest->settings);
	free(manifest);
}