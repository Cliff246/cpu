#include "emiter.h"
#include "commons.h"
#include "module.h"
#include "eerror.h"

region_t *create_region(module_t *mod)
{
	region_t *region = CALLOC(1, region_t);
	region->mod = mod;
	size_t modsize = total_module_size(region->mod);
	region->size = modsize;
	return region;
}

outorder_t *create_outorder(linker_t *ll)
{
	outorder_t *oo = CALLOC(1, outorder_t);

	size_t count = 0;

	for(int mi = 0; mi < MAX_TAGS; ++mi)
	{
		if(ll->modules[mi].filled)
			count++;
		else
			break;
	}

	oo->regions = CALLOC(count, region_t *);
	oo->count = count;
	size_t total_size = 0;
	for(int ri = 0; ri < count; ++ri)
	{
		module_t *mod = &ll->modules[ri];

		region_t *re = create_region(mod);
		oo->regions[ri] = re;
		print_module(re->mod);
		printf("size %d\n", re->size);

		total_size += re->size;
	}

	return oo;
}


void set_outorder_positions(outorder_t *oo, int *positions, size_t size)
{
	if(size != oo->count)
	{
		printf("output order size does not match modules count %d %d\n", size, oo->count);
		exit(1);
	}
	//this is going to be packed
	region_t *temp[size];
	//this should sort
	//checking the system
	for(int i = 0; i < size; ++i)
	{
		int spot = positions[i];
		if(spot < 0 || spot >= size)
		{
			printf("order at [%d] = %d is outside of valid range\n", i, spot);
			exit(1);
		}
		oo->regions[i]->order = i;
		printf("%d %p\n", i, oo->regions[i]);
		temp[spot] = oo->regions[i];
	}

	memcpy(oo->regions, temp, sizeof(region_t *) * size);





}


void resolve_positions(outorder_t *oo)
{
	//this is soooo bad
	size_t position = 0;
	for(int i = 0; i < oo->count; ++i)
	{
		printf("%d %p\n", i, oo->regions[i]);
		oo->regions[i]->position = position;
		printf("%d\n", position);

		position += oo->regions[i]->size;
	}



}


segout_txt_t create_segout_txt(linker_t *ll, region_t *region)
{
	module_t *mod = region->mod;
	size_t inst_len = 0;
	size_t imm_len = 0;

	for(int fi = 0; fi < mod->size; ++fi)
	{
		modfrag_t *frag = &mod->fragments[fi];
		modfrag_code_t code = frag->frag.code;
		inst_len += code.insts;
		imm_len += code.imms;
	}


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
		//printf("enty %d %d\n", i, inst_len);
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
				alias_t *ali = (alias_t *)getdata_from_hash_table(ctx->alias_map, instruction->immref);
				if(!ali)
				{

					errelm_line_t line = {.column = entry->node->tok->locale.col, .line = entry->node->tok->locale.row};
					errelm_file_t file = {.name = get_path_from_identifier(entry->node->tok->locale.file)};
					char buffer[1025] = {0};
					sprintf(buffer, "could not find %s in reference table", instruction->immref);

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
					if(!(ali->symbol->symbol.ref->resolved))
					{
						perror("reference has not been resolved and is global\n");
						exit(1);
					}
					current_imm = ali->symbol->symbol.ref->resolved_address;

				}
				else if(instruction->ref_type == INST_REF_LOCAL)
				{
					current_imm = ali->symbol->symbol.ref->locale_offset;
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

segout_data_t create_segout_data(linker_t *ll, region_t *region)
{



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
			data_holder_t holder = mop->data;
			for(int wi = 0; wi < holder.words_len; ++wi)
			{
				//printf("%16x holder words\n",holder.words[wi]);
				content[iter++] = holder.words[wi];

			}
		}


	}
	data.data = content;
	data.data_size = iter;

	return data;
}



segout_t create_segout(linker_t *ll, region_t *region)
{

	segout_t out = {0};
	switch(region->mod->type)
	{
		case MODULE_CODE:

			out.output.txt = create_segout_txt();
			break;
		case MODULE_DATA:
			out.output.data = create_segout_data();
			break;




		default:

			break;
	}
	out.type = region->mod->type;
	return out;

}


void emit_segouts(linker_t *ll, outorder_t *oo)
{


	for(int i = 0; i < oo->count; ++i)
	{
		region_t *region = oo->regions[i];
	}


}