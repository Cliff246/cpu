#include "encoder.h"

#include "string.h"
#include "commons.h"
#include "eerror.h"
#include "errno.h"


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

	//printf("immedates %d instructions %d\n", immedates, instructions);

	return size + excess + table;

}

uint64_t realloc_resolve(iscope_t *re)
{
	return 0;
}

uint64_t data_resolve(iscope_t *data)
{
	//TODO

	uint64_t size = 0;
	for(int i = 0; i < data->entries_count; ++i )
	{
		ientry_t *entry = data->entries[i];


		if(entry->type == IE_MOP)
		{
			size += entry->entry.mop.holder.words_len;
		}
		else
		{
			//TODO error out
		}

	}

	return size;
}


segout_txt_t create_segout_txt(icontext_t *ctx, iscope_t *scope, style_t *style)
{
	const size_t inst_len = scope->entries_count;

	const size_t table_size = inst_len/ 128 + 1;
	uint64_t *table = CALLOC(table_size, uint64_t);
	//oversized
	int64_t *imm = CALLOC(inst_len, int64_t);
	int64_t *inst = CALLOC(inst_len, int64_t);


	segout_txt_t txt = {0};
	txt.imm = imm;
	txt.imm_len = 0;

	size_t imm_iter = 0;
	size_t table_iter = 0;


	for(int i = 0; i < inst_len; ++i)
	{
		//printf("enty %d %d\n", i, inst_len);
		ientry_t *entry = scope->entries[i];

		if(imm_iter % 128 == 0)
		{
			table[table_iter++] = imm_iter;
		}

		if(entry->type != IE_INST)
		{
			printf("must be only instructions");
			exit(1);
		}
		inst_t *instruction = &entry->entry.inst;
		printf("content %s\n", entry->node->tok->lexeme);
		print_inst(instruction);

		if(instruction->immflag )
		{
			int64_t current_imm = 0;
			if(instruction->immref != NULL)
			{
				//should do local and global conversion here
				iref_t *ref = (iref_t *)getdata_from_hash_table(ctx->ref_table, instruction->immref);
				if(!ref)
				{
					print_hash_table(ctx->ref_table);

					perror("ref not found\n");
					exit(1);
				}
				if(instruction->ref_type == INST_REF_GLOBAL)
				{
					if(!ref->resolved)
					{
						perror("reference has not been resolved and is global\n");
						exit(1);
					}
					current_imm = ref->resolved_address;

				}
				else if(instruction->ref_type == INST_REF_LOCAL)
				{
					current_imm = ref->offset;
				}
				else
				{
					perror("reference has not a valid type");
					exit(1);
				}

			}
			else
			{
				current_imm = instruction->imm;
			}

			imm[imm_iter ++] = current_imm;
			//printf("current imm %d\n", current_imm);
		}
		//this is an undersized type need to be packed later
		//todo
		int32_t current_inst = encode_inst(instruction);
		//undersized;
		inst[i] = current_inst;

	}
 	size_t base = style->regions[scope->segment->segment_id].position;

	txt.desc[0] = 6 + base;
	txt.desc[1] = table_size;

	const size_t inst_offset = 6 + base + table_size;
	txt.desc[2] = inst_offset;
	txt.desc[3] = inst_len;
	const size_t imm_offset = inst_offset + inst_len;
	txt.desc[4] = imm_offset;
	txt.desc[5] = imm_iter;

	txt.inst = inst;
	txt.inst_len = inst_len;
	txt.imm = imm;
	txt.imm_len = imm_iter;
	txt.table = table;
	return txt;
}

segout_data_t create_segout_data(icontext_t *ctx, iscope_t *scope, style_t *style)
{
	segout_data_t data = {0};

 	size_t size = style->regions[scope->segment->segment_id].size;
	uint64_t *content = CALLOC(size, uint64_t);

	size_t iter = 0;
	for(int i = 0; i < scope->entries_count; ++i)
	{
		ientry_t *entry = scope->entries[i];

		if(entry->type != IE_MOP)
		{
			perror("entry must be mop in data section");
			exit(1);
		}

		mop_t *mop = &entry->entry.mop;

		for(int wi = 0; wi < mop->holder.words_len; ++wi)
		{
			content[iter++] = mop->holder.words[wi];

		}

	}
	data.data = content;
	data.data_size = iter;

	return data;
}

segout_t create_segout(icontext_t *ctx, int segindex, style_t *style)
{
	iscope_t *scope = &ctx->scopes[segindex];
	segout_t out = {0};

	switch(scope->segment->segtype)
	{
		case ISEG_TEXT:

			out.output.txt = create_segout_txt(ctx, scope, style);
			break;
		case ISEG_DATA:
			out.output.data = create_segout_data(ctx, scope, style);
			break;

		default:

			break;
	}
	out.type = scope->segment->segtype;

	return out;

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

			for(int i_inst = 0; i_inst < txt->inst_len; ++i_inst)
			{
				bin[bin_iter ++] = txt->inst[i_inst];
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


uint64_t resolve_size(iscope_t *scope)
{
	uint64_t size = 0;

	switch(scope->segment->segtype)
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



size_t resolve_addresses(icontext_t *ctx,  style_t *style)
{


	region_t *regions = CALLOC(ctx->scopes_count, region_t);

	size_t current_postion = 0;

	for(int i = 0; i < ctx->scopes_count; ++i)
	{
		 size_t region_size = resolve_size(&ctx->scopes[i]);
		 regions[i].size = region_size;

		 regions[i].position = current_postion;

		 current_postion += region_size;
		 //printf("region: %d size:%lu pos:%lu\n", i, region_size, regions[i].position);
	}

	style->regions = regions;
	style->regions_size = current_postion;
	return current_postion;
}

void fill_in_addresses(icontext_t *ctx, style_t *style)
{
	size_t current_size = 0;
	//regions
	for(int i = 0; i < ctx->scopes_count; ++i)
	{
		size_t scope_base_offset = 0;

		iscope_t *scope = &ctx->scopes[i];


		//table space in scope .text
		if(scope->segment->segtype == ISEG_TEXT)
		{
			//base offset
			scope_base_offset = (scope->entries_count / 128) + 1 + 6;
		}

		//references
		for(int r = 0; r < scope->refs_count; ++r)
		{
			iref_t *ref = scope->refs[r];
			ref->resolved_address = ref->offset + scope_base_offset + current_size;
			ref->resolved = true;
			//printf("resolved: %s %d \n", ref->ref_string, ref->resolved_address);

		}
		//printf("fill in\n");

		current_size += style->regions[i].size;
	}

}

output_t *emit(icontext_t *ctx)
{
	style_t style = {0};

	size_t total_size = resolve_addresses(ctx, &style);
	fill_in_addresses(ctx, &style);

	segout_t *segouts = CALLOC(ctx->scopes_count, segout_t);

	for(int i = 0; i < ctx->scopes_count; ++i)
	{
		segouts[i] = create_segout(ctx, i, &style);
	}
	return combine_segouts(segouts, ctx->scopes_count);
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
		printf("%.3d: %.8llx %lld\n", i, output->bin[i], output->bin[i]);



	}
	fwrite(output->bin, sizeof(uint64_t), output->size, fp);
	fclose(fp);
}
