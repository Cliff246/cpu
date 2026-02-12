#ifndef __CFG_MANIFEST_HEADER__
#define __CFG_MANIFEST_HEADER__

#include "CFG_entry.h"
#include "CFG_setting.h"
#include "IO_parser.h"
#include <stdint.h>


typedef struct CFG_manifest
{
	uint64_t size;
	CFG_entry_t **entries;

	uint64_t settings_size;
	CFG_setting_t **settings;
}CFG_manifest_t;


uint64_t CFG_size_manifest(IO_ptree_t *tree);
CFG_manifest_t *CFG_init_manifest(IO_ptree_t *tree);
void CFG_free_manifest(CFG_manifest_t *manifest);


#endif