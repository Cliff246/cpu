#include "encoder.h"

#include "string.h"
#include "commons.h"


uint64_t text_resolve(iscope_t *txt)
{

	uint64_t instructions = 0;
	uint64_t immedates = 0;
	for(int i = 0; i < txt->entries_count; ++i)
	{
		ientry_t *entry = txt->entries[i];
		if(entry->type == IE_INST)
		{
			if(entry->entry.inst.immflag != 0)
			{
				immedates++;
			}
			instructions++;

		}


	}
	uint64_t size = instructions + immedates;
	uint64_t excess = (instructions / 128) + 1;
	const uint64_t table = 6;

	printf("immedates %d instructions %d\n", immedates, instructions);

	return size + excess + table;

}


uint64_t resolve_size(iscope_t *scope)
{
	uint64_t size = 0;

	switch(scope->segment->segtype)
	{

		case ISEG_TEXT:
			size = text_resolve(scope);
			printf("text segment: %d\n", size);
			break;

		default:
			break;

	}
	return size;
}



void resolve_addresses(icontext_t *ctx)
{


	region_t *regions = CALLOC(ctx->scopes_count, region_t);

	size_t current_postion = 0;

	for(int i = 0; i < ctx->scopes_count; ++i)
	{
		 size_t region_size = resolve_size(&ctx->scopes[i]);
		 regions[i].size = region_size;

		 regions[i].position = current_postion;

		 current_postion += region_size;
		 printf("region: %d size:%lu pos:%lu\n", i, region_size, regions[i].position);
	}


}
