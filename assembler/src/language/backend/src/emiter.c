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

void pass_assign_addresses(outorder_t *oo)
{


}

symbol_t *resolve_symbol(char *key, linker_t *lk, modfrag_t *frag)
{

	context_t *ctx = get_context_from_ref(lk, frag->ref);
	printf("local %p\n", ctx);
	alias_t *ali = (alias_t *)getdata_from_hash_table(ctx->alias_map, key);
	if(ali)
	{
		return ali->symbol;
	}
	printf("global %p\n", ctx);


	global_t *glb = (global_t *)getdata_from_hash_table(lk->globals, key);
	printf("global %d\n", glb->type);
	if(glb && glb->type == GLOBAL_SYMBOL)
	{

		symbol_t *sym = get_symbol_from_global(lk, glb);
		return sym;

	}
	else
	{

		LOG("tried to resolve symbol that's not valid %s\n", key);

		return NULL;
	}
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
		if(so->type == MODULE_CODE)
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
		else if(so->type == MODULE_DATA)
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

	printf("lands\n");
	const size_t table_size = inst_len/ 128 + 1;
	uint64_t *table = CALLOC(table_size, uint64_t);
	//oversized
	int64_t *imm = CALLOC(inst_len, int64_t);
	int64_t *inst = CALLOC(inst_len, int64_t);


	segout_txt_t txt = {0};
	txt.imm = imm;
	txt.imm_len = 0;

	size_t imm_iter = 0;
	size_t inst_iter = 0;
	size_t table_iter = 0;
	bool failed = false;



	for(int fi = 0; fi < mod->size; ++fi)
	{

		modfrag_t *frag = &mod->fragments[fi];
		scope_t *scope = get_scope_from_ref(ll, frag->ref);
		printf("run\n");
		for(int si = 0; si < scope->entries.count; ++si)
		{
			//printf("enty %d %d\n", si, scope->entries.count);
			entry_t *entry = scope->entries.entries[si];

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
					symbol_t *sym = resolve_symbol(instruction->immref, ll, frag);





					if(!sym)
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
						if(sym->symbol.ref->resolved == false)
						{
							perror("reference has not been resolved and is global\n");
							exit(1);
						}

						ref_t *ref = sym->symbol.ref;

						current_imm = ref->fragment_offset + ref->locale_offset + ref->absolute_offset;

					}
					else if(instruction->ref_type == INST_REF_LOCAL)
					{
						ref_t *ref = sym->symbol.ref;

						current_imm = ref->fragment_offset + ref->locale_offset;

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
			inst[inst_iter++] = current_inst;

		}

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

	module_t *mod = region->mod;



	segout_data_t data = {0};

 	size_t size = region->size;
	uint64_t *content = CALLOC(size, uint64_t);

	size_t iter = 0;
	for(int fi = 0; fi < mod->size; ++fi)
	{
		modfrag_t *frag = &mod->fragments[fi];
		scope_t *scope = get_scope_from_ref(ll, frag->ref);
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
	}

	data.data = content;
	data.data_size = iter;

	return data;
}



segout_t create_segout(linker_t *ll, region_t *region)
{

	segout_t out = {0};
	printf("output %d\n", region->mod->type);
	switch(region->mod->type)
	{
		case MODULE_CODE:

			out.output.txt = create_segout_txt(ll, region);
			break;
		case MODULE_DATA:
			out.output.data = create_segout_data(ll, region);
			break;




		default:

			break;
	}
	out.type = region->mod->type;
	return out;

}


void print_segout_code(segout_txt_t *code)
{

	printf("table ptr %d\n", code->desc[0]);
	printf("table len %d\n", code->desc[1]);
	printf("inst ptr  %d\n", code->desc[2]);
	printf("inst len  %d\n", code->desc[3]);
	printf("imm ptr   %d\n", code->desc[4]);
	printf("imm len   %d\n", code->desc[5]);

	for(int ti = 0; ti < code->desc[1]; ++ti)
	{
		printf("table: %d\n", code->table[ti]);
	}

	for(int ii = 0; ii < code->inst_len; ++ii)
	{
		printf("%.3d: %.16llx %lld\n", ii, code->inst[ii], code->inst[ii]);

	}
	for(int iim = 0; iim < code->imm_len; ++iim)
	{
		printf("%.3d: %.16llx %lld\n", iim, code->imm[iim], code->imm[iim]);

	}
}

void print_segout_data(segout_data_t *data)
{
	for(int i = 0; i < data->data_size; ++i)
	{
		printf("%.3d: %.16llx %lld\n", i, data->data[i], data->data[i]);
	}

}

void print_segout(segout_t *out)
{
	switch(out->type)
	{
		case MODULE_CODE:
			print_segout_code(&out->output.txt);
			break;
		case MODULE_DATA:
			print_segout_data(&out->output.data);
			break;
		default:
			LOG("invalid segout type\n", 0);
			break;
	}
}

size_t fix_align_scope_addresses(scope_t *scope, size_t module_offset, size_t fragment_offset)
{
	size_t used = get_scope_size(scope);
	for(int si = 0; si < scope->symbols.count; ++si)
	{
		symbol_t *sym = scope->symbols.symbols[si];

		if(sym->type == SYMBOL_REFERENCE)
		{
			ref_t *ref = sym->symbol.ref;

			ref->fragment_offset = fragment_offset;
			ref->absolute_offset = module_offset;
			ref->resolved = true;

		}
		else if(sym->type == SYMBOL_INVAL)
		{
			LOG("symbol type invalid %s\n", sym->key, 0);
			exit(EXIT_FAILURE);
		}
		else
		{
			LOG("symbol type not implemented %d %s\n ", sym->type, sym->key);
			exit(EXIT_FAILURE);
		}
	}
	return used;
}

void fix_align_module_addresses(linker_t *ll, module_t *mod, size_t offset)
{
	size_t fragment_offset = 0;
	for(int mi = 0; mi < mod->size; ++mi)
	{
		modfrag_t *frag = &mod->fragments[mi];
		scope_t *scope = get_scope_from_ref(ll, frag->ref);
		size_t scope_local = fix_align_scope_addresses(scope, offset, fragment_offset);
		fragment_offset += scope_local;
		print_scope_symbols(scope);

	}

}


void fix_addresses(linker_t *ll, outorder_t *oo)
{

	for(int ri = 0; ri < oo->count; ++ri)
	{
		region_t *region = oo->regions[ri];

		fix_align_module_addresses(ll, region->mod, region->position);
	}

}

void emit_segouts(linker_t *ll, outorder_t *oo)
{


	for(int ri = 0; ri < oo->count; ++ri)
	{
		region_t *region = oo->regions[ri];


	}


}





void print_over_fragment(linker_t *lk, modfrag_t *frag)
{


	scope_t *scope = get_scope_from_ref(lk, frag->ref);

	for(int ei = 0; ei < scope->entries.count; ei++)
	{
		entry_t *entry = scope->entries.entries[ei];
		print_entry(entry);
	}

}



void print_over_module(linker_t *lk, module_t *mod)
{

	//module iter
	for(int mi = 0; mi < mod->size; mi++)
	{
		modfrag_t *frag = &mod->fragments[mi];
		print_over_fragment(lk, frag);

	}
}


void print_over_outorder(linker_t *lk, outorder_t *oo)
{

	//ri = region iterator
	for(int ri = 0; ri < oo->count; ++ri)
	{
		region_t *region = oo->regions[ri];

		print_over_module(lk, region->mod);
	}


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
