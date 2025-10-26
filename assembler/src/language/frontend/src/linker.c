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
	for(int i = 0; i < link->ctx->locales.size; ++i)
	{
		scope_t *scope = &link->ctx->locales.locales[i].scope;
		if(scope->segment.segtype == ISEG_CONFIG)
		{
			has = true;
		}
	}
	return has;
}






void add_link_to_linker(linker_t *lctx, link_t *link)
{
	if(!lctx->links)
	{
		lctx->links = CALLOC(1, link_t *);
		lctx->links[lctx->length] = link;

	}
	else
	{
		lctx->links = REALLOC(lctx->links, lctx->length + 1, link_t*);
		lctx->links[lctx->length] = link;

	}
	lctx->length++;

}

global_t *create_global(char *key, link_t *link)
{
	global_t *global = CALLOC(1, global_t);
	global->src = link;
	global->key = strdup(key);
	return global;
}

void free_global(void *ptr)
{
	global_t *gptr = (global_t *)ptr;
	free(gptr->key);

}

void get_globals_from_link(linker_t *lctx, link_t *link)
{


	if(!link->ctx)
	{
		//FAIL OUT
		perror("link invalid");
		exit(1);
	}


	for(int i = 0; i < link->ctx->locales.size; ++i)
	{

		scope_t *scope = &link->ctx->locales.locales[i].scope;
		if(scope->segment.segtype == ISEG_CONFIG)
		{


			for(int exposed = 0; exposed < scope->entries.count; exposed++)
			{
				entry_t *entry = scope->entries.entries[exposed];
				if(entry->type == ENTRY_MOP)
				{
					mop_t *mop =  &entry->entry.mop;
					if(mop->type == MOP_TYPE_DEFINE_CONFIG)
					{
						if(mop->holder.config.type == MOP_CONFIG_TYPE_PUBLIC)
						{
							//do something to include that into public
							global_t *gptr = create_global(mop->holder.config.config.pub.key_string, link);
							int64_t i = addto_hash_table(lctx->globals, mop->holder.config.config.pub.key_string, (void *)gptr);

						}

					}

				}

			}
		}
	}


}


link_t *create_link(context_t *ictx)
{
	link_t *link = CALLOC(1, link_t);
	link->ctx = ictx;
	return link;
}

linker_t *create_linker(void)
{
	linker_t *lctx = CALLOC(1, linker_t);

	lctx->globals = new_hash_table(1000, free_global);
	return lctx;
}
