#include "linker.h"
#include "commons.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

bool has_config_segment(link_t *link)
{
	bool has = false;
	for(int i = 0; i < link->ctx->scopes_count; ++i)
	{
		iscope_t *scope = &link->ctx->scopes[i];
		if(scope->segment->segtype == ISEG_CONFIG)
		{
			has = true;
		}
	}
	return has;
}






void add_link_to_linker(linker_t *lctx, icontext_t *ctx)
{
	if(!lctx->links)
	{
		lctx->links = CALLOC(lctx->length, link_t);
		lctx->links[lctx->length].ctx = ctx;

	}
	else
	{
		lctx->links = REALLOC(lctx->links, lctx->length + 1, link_t);
		lctx->links[lctx->length].ctx = ctx;

	}
	lctx->length++;

}

void free_global(void *ptr)
{
	global_t *gptr = (global_t *)ptr;


}

void get_globals_from_link(linker_t *lctx, link_t *link)
{


	if(!link->ctx)
	{
		//FAIL OUT
		perror("link invalid");
		exit(1);
	}


	for(int i = 0; i < link->ctx->scopes_count; ++i)
	{

		iscope_t *scope = &link->ctx->scopes[i];
		if(scope->segment->segtype == ISEG_CONFIG)
		{


			for(int exposed = 0; exposed < scope->entries_count; exposed++)
			{
				ientry_t *entry = scope->entries[exposed];
				if(entry->type == IE_MOP)
				{
					mop_t *mop =  &entry->entry.mop;
					if(mop->type == MOP_TYPE_DEFINE_CONFIG)
					{
						if(mop->mop == MOP_CONFIG_TYPE_PUBLIC)
						{
							//do something to include that into public

						}
					}

				}

			}
		}
	}


}

linker_t *create_linker(void)
{
	linker_t *lctx = CALLOC(1, linker_t);

	lctx->globals = new_hash_table(1000, free_global);

}
