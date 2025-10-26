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
		printf("%d\n", i);

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
			if(entry->entry.mop.type == MOP_TYPE_DEFINE_CONFIG)
			{
				//do shit with config
			}
			else
			{
				//error out
			}
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
				size += mop->holder.data.words_len;

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


segout_txt_t create_segout_txt(context_t *ctx, int index)
{
	scope_t *scope = &ctx->locales.locales[index].scope;
	const size_t inst_len = scope->entries.count;

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
	bool failed = false;

	for(int i = 0; i < inst_len; ++i)
	{
		printf("enty %d %d\n", i, inst_len);
		entry_t *entry = scope->entries.entries[i];

		if(imm_iter % 128 == 0)
		{
			table[table_iter++] = imm_iter;
		}

		if(entry->type != ENTRY_INST)
		{
			printf("must be only instructions");
			exit(1);
		}
		inst_t *instruction = &entry->entry.inst;
		//printf("content %s\n", entry->node->tok->lexeme);
		//print_inst(instruction);

		if(instruction->immflag )
		{
			int64_t current_imm = 0;
			if(instruction->immref != NULL)
			{
				//should do local and global conversion here
				symbol_t *sym = (symbol_t *)getdata_from_hash_table(ctx->symmap, instruction->immref);
				if(!sym)
				{

					errelm_line_t line = {.column = entry->node->tok->locale.col, .line = entry->node->tok->locale.row};
					errelm_file_t file = {.name = get_path_from_identifier(entry->node->tok->locale.file)};
					char buffer[1025] = {0};
					sprintf(buffer, "could not fine %s in reference table", instruction->immref);

					errelm_t elmline = errelm_create_line_element(line);
					errelm_t elmfile =  errelm_create_file_element(file);
					//printf("emit stage\n");
					emit_error(KEY_ERROR, buffer, 2, elmfile, elmline);
					//printf("emit stage2\n");

					failed = true;
					continue;
				}
				if(instruction->ref_type == INST_REF_GLOBAL)
				{
					if(!(sym->ref->resolved))
					{
						perror("reference has not been resolved and is global\n");
						exit(1);
					}
					current_imm = sym->ref->resolved_address;

				}
				else if(instruction->ref_type == INST_REF_LOCAL)
				{
					current_imm = sym->ref->locale_offset;
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
	region_t *region = &ctx->locales.locales[index].region;

 	size_t base = region->position;


	if(failed == true)
	{
		print_errors();
		exit(1);
	}

	size_t compacted = (inst_len / 2) + (inst_len % 2);

	txt.desc[0] = 6 + base;
	txt.desc[1] = table_size;

	const size_t inst_offset = 6 + base + table_size;
	txt.desc[2] = inst_offset;
	txt.desc[3] = compacted;
	const size_t imm_offset = inst_offset + compacted;
	txt.desc[4] = imm_offset;
	txt.desc[5] = imm_iter;

	txt.inst = inst;
	txt.inst_len = inst_len;
	txt.imm = imm;
	txt.imm_len = imm_iter;
	txt.table = table;
	return txt;
}

segout_data_t create_segout_data(context_t *ctx, int index)
{
	scope_t *scope = &ctx->locales.locales[index].scope;
	region_t *region = &ctx->locales.locales[index].region;

	segout_data_t data = {0};

 	size_t size = region->size;
	uint64_t *content = CALLOC(size, uint64_t);

	size_t iter = 0;
	for(int i = 0; i < scope->entries.count; ++i)
	{
		entry_t *entry = scope->entries.entries[i];

		if(entry->type != ENTRY_MOP)
		{
			perror("entry must be mop in data section");
			exit(1);
		}

		mop_t *mop = &entry->entry.mop;
		if(mop->type == MOP_TYPE_DEFINE_DATA)
		{
			data_holder_t holder = mop->holder.data;
			for(int wi = 0; wi < holder.words_len; ++wi)
			{
				printf("%16x holder words\n",holder.words[wi]);
				content[iter++] = holder.words[wi];

			}
		}


	}
	data.data = content;
	data.data_size = iter;

	return data;
}



segout_t create_segout(context_t *ctx, int segindex)
{
	scope_t *scope = &ctx->locales.locales[segindex].scope;
	segout_t *out = &ctx->locales.locales[segindex].out;

	switch(scope->segment.segtype)
	{
		case ISEG_TEXT:

			out->output.txt = create_segout_txt(ctx, segindex);
			break;
		case ISEG_DATA:
			out->output.data = create_segout_data(ctx, segindex);
			break;




		default:

			break;
	}
	out->type = scope->segment.segtype;

	return *out;

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
					print_inst(&inst2);
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
				printf("%x\n",data->data[i_data]);
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
	printf("resolve %d\n", scope->segment.segtype);
	switch(scope->segment.segtype)
	{

		case ISEG_TEXT:
			size = text_resolve(scope);
			printf("text segment: %d\n", size);
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
		printf("counter %d %d\n", i, ctx->locales.size);
		 size_t region_size = resolve_size(&ctx->locales.locales[i].scope);
		 ctx->locales.locales[i].region.size = region_size;
		 ctx->locales.locales[i].region.position = region_size;
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
		for(int r = 0; r < scope->refs.count; ++r)
		{
			ref_t *ref = scope->refs.refs[r];
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
	printf("resolve addresses\n");
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
