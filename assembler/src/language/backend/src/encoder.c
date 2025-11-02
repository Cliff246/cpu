#include "encoder.h"

#include "string.h"
#include "commons.h"
#include "eerror.h"
#include "errno.h"
#include "entry.h"

uint64_t text_resolve(scope_t *txt)
{

	uint64_t instructions = 0;
	uint64_t immedates = 0;
	for(int i = 0; i < txt->entries.count; ++i)
	{
		//printf("%d\n", i);

		entry_t *entry = txt->entries.entries[i];
		if(entry->type == ENTRY_INST)
		{
			if(entry->entry.inst.immflag != 0)
			{
				immedates++;
			}
			instructions++;

		}
		else if(entry->type == ENTRY_MOP)
		{

		}


	}
	uint64_t actual_instructions = (instructions / 2) + (instructions % 2);
	uint64_t size = actual_instructions + immedates;

	uint64_t excess = (instructions / 128) + 1;
	const uint64_t table = 6;

	printf("sze %d instructions %d\n", size, excess);

	return size + excess + table;

}



uint64_t realloc_resolve(scope_t *re)
{
	return 0;
}

uint64_t data_resolve(scope_t *data)
{
	//TODO

	uint64_t size = 0;
	for(int i = 0; i < data->entries.count; ++i )
	{
		entry_t *entry = data->entries.entries[i];


		if(entry->type == ENTRY_MOP)
		{
			mop_t *mop = &entry->entry.mop;
			if(mop->type == MOP_TYPE_DEFINE_DATA)
			{
				size += mop->data.words_len;

			}
			else
			{
				//Not allowed
			}
		}
		else
		{
			//TODO error out
		}

	}

	return size;
}






output_t *combine_segouts(segout_t *segouts, int length)
{

	output_t *output = CALLOC(1, output_t);

	size_t bin_alloc = 100;
	int64_t *bin = CALLOC(bin_alloc, uint64_t);
	size_t bin_iter = 0;

	for(int i = 0; i < length; ++i)
	{



		segout_t *so = &segouts[i];
		//printf("type: %d alloc: %d\n", so->type, bin_alloc);
		if(so->type == ISEG_TEXT)
		{
			segout_txt_t *txt = &so->output.txt;
			const size_t table_len = (txt->inst_len / 128) + 1;

			size_t added = txt->imm_len + txt->inst_len + table_len + 6;
			if(added + bin_iter >= bin_alloc)
			{
				bin_alloc += added;
				bin = REALLOC(bin, bin_alloc, uint64_t);
			}
			for(int i_desc = 0; i_desc < 6; ++i_desc)
			{
				bin[bin_iter++] = txt->desc[i_desc];
			}

			for(int i_table = 0; i_table < table_len; ++i_table)
			{
				bin[bin_iter++] = txt->table[i_table];
			}

			for(int i_inst = 0; i_inst < txt->inst_len; i_inst++)
			{
				inst_t inst = decode_inst(txt->inst[i_inst]);
				//print_inst(&inst);

				//add 1

				uint64_t compressed = txt->inst[i_inst++];
				compressed <<= 32;
				//add 2
				if(i_inst < txt->inst_len)
				{
					inst_t inst2 = decode_inst(txt->inst[i_inst]);
					//print_inst(&inst2);
					compressed += txt->inst[i_inst];

				}
				bin[bin_iter ++] = compressed;
			}


			for(int i_imm = 0; i_imm < txt->imm_len; ++i_imm)
			{
				bin[bin_iter++] = txt->imm[i_imm];
			}

		}
		else if(so->type == ISEG_DATA)
		{
			segout_data_t *data = &so->output.data;
			if(data->data_size + bin_iter >= bin_alloc)
			{
				bin_alloc += (data->data_size);
				bin = REALLOC(bin, bin_alloc, uint64_t);
			}
			for(int i_data = 0; i_data < data->data_size; ++i_data)
			{
				//printf("%x\n",data->data[i_data]);
				bin[bin_iter++] = data->data[i_data];
			}

		}


		else
		{
			perror("segout type emit not done yet\n");
			exit(1);
		}
	}
	output->bin = bin;
	output->size = bin_iter;
	return output;
}


uint64_t resolve_size(scope_t *scope)
{
	uint64_t size = 0;
	//printf("resolve %d\n", scope->segment.segtype);
	switch(scope->segment.segtype)
	{

		case ISEG_TEXT:
			size = text_resolve(scope);
			//printf("text segment: %d\n", size);
			break;

		case ISEG_REALLOC:
			size = realloc_resolve(scope);
			break;
		case ISEG_DATA:
			size = data_resolve(scope);
			break;
		default:
			break;

	}
	return size;
}



size_t resolve_addresses(context_t *ctx)
{



	size_t current_postion = 0;

	for(int i = 0; i < ctx->locales.size; ++i)
	{
		//printf("counter %d %d\n", i, ctx->locales.size);
		size_t region_size = resolve_size(&ctx->locales.locales[i].scope);
		ctx->locales.locales[i].region.size = region_size;
		ctx->locales.locales[i].region.position = current_postion;
		current_postion += region_size;
	}

	return current_postion;
}

void fill_in_addresses(context_t *ctx)
{
	size_t current_size = 0;
	//regions
	for(int i = 0; i < ctx->locales.size; ++i)
	{
		size_t scope_base_offset = 0;

		scope_t *scope = &ctx->locales.locales[i].scope;


		//table space in scope .text
		if(scope->segment.segtype == ISEG_TEXT)
		{
			//base offset
			scope_base_offset = (scope->entries.count / 128) + 1 + 6;
		}

		//references
		for(int r = 0; r < scope->symbols.count; ++r)
		{
			symbol_t *sym = scope->symbols.symbols[r];
			ref_t *ref = sym->symbol.ref;
			ref->resolved_address = ref->locale_offset + scope_base_offset + current_size;
			ref->resolved = true;
			//printf("resolved: %s %d \n", ref->ref_string, ref->resolved_address);

		}
		//printf("fill in\n");

		current_size += ctx->locales.locales[i].region.size;
	}

}

output_t *emit(context_t *ctx)
{
	//printf("resolve addresses\n");
	size_t total_size = resolve_addresses(ctx);
	fill_in_addresses(ctx);

	segout_t *segouts = CALLOC(ctx->locales.size, segout_t);

	for(int i = 0; i < ctx->locales.size; ++i)
	{
		segouts[i] = create_segout(ctx, i);
	}

	return combine_segouts(segouts, ctx->locales.size);
}



void write_out(output_t *output, char *name)
{
	FILE *fp = fopen(name, "wb");
	if(fp == NULL)
	{
		ERR(EBADF, "could not load file");
		exit(1);
	}
	for(int i = 0; i < output->size; i++)
	{
		printf("%.3d: %.16llx %lld\n", i, output->bin[i], output->bin[i]);



	}
	fwrite(output->bin, sizeof(uint64_t), output->size, fp);
	fclose(fp);
}
