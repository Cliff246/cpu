#include "module.h"
#include "commons.h"
#include "linker.h"
#include <stdlib.h>
#include <stdio.h>

static modfrag_code_t module_code_resolve(scope_t *txt)
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





	modfrag_code_t code = {.imms = immedates, .insts = instructions};
	return code;

}




static modfrag_data_t module_data_resolve(scope_t *scope)
{
	//TODO

	uint64_t size = 0;
	for(int i = 0; i < scope->entries.count; ++i )
	{
		entry_t *entry = scope->entries.entries[i];


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

	modfrag_data_t data = {.words = size};
	return data;
}

static module_type_t segment_type_to_module_type(seg_type_t type)
{
	switch(type)
	{
		case ISEG_CODE:
			return MODULE_CODE;
		case ISEG_TEXT:
			return MODULE_CODE;
		case ISEG_DATA:
			return MODULE_DATA;
		default:
			return MODULE_INVAL;
	}
}

bool scope_is_of_correct_type(module_type_t type, scope_t *scope)
{

	if((scope->segment.segtype == ISEG_CODE || scope->segment.segtype == ISEG_TEXT) && type == MODULE_CODE)
	{
		return true;
	}
	else if(scope->segment.segtype == ISEG_DATA && type == MODULE_DATA)
	{
		return true;
	}
	else
		return false;
}




void append_scope_ref(struct linker *lk, module_t *mod, int ctx_id, int local_index)
{
	if(mod->fragments == NULL)
	{
		mod->set = true;
		mod->fragments = CALLOC(1, modfrag_t);

	}
	else
	{
		mod->fragments = REALLOC(mod->fragments, mod->size + 1, modfrag_t);

	}
	modfrag_t *mf = &mod->fragments[mod->size];
	mf->ref.ctx_id = ctx_id;
	mf->ref.locale_index = local_index;
	module_type_t current =  segment_type_to_module_type(get_scope_from_ref(lk, mf->ref)->segment.segtype);

	if(mod->type == MODULE_UNSET)
	{
		mod->type = current;
	}
	else
	{
		if(mod->type != current)
		{
			printf("module type must be consistent across all fragments\n");
			exit(1);
		}

	}
	printf("test %d\n", current);

	mod->size++;
}

void fill_fragment_code(scope_t *scope, modfrag_t *frag)
{



	frag->frag.code = module_code_resolve(scope);
}

void fill_fragment_data(scope_t *scope, modfrag_t *frag)
{


	frag->frag.data = module_data_resolve(scope);



}

void fill_fragment(struct linker *lk, module_t *in, int index)
{
	modfrag_t *frag = &in->fragments[index];

	scope_t *scope = get_scope_from_ref(lk, frag->ref);




	switch(in->type)
	{
		case MODULE_CODE:
			fill_fragment_code(scope, frag);
			break;
		case MODULE_DATA:
			fill_fragment_data(scope, frag);
			break;
		default:
			printf("fragment inval\n");
			break;
	}


}

module_t *get_module(struct linker *lk, int sid)
{
	module_t *module = &lk->modules[segment_ids_to_tag[sid]];
	return module;

}
static void print_scope_ref(scope_ref_t sr)
{
	printf("	ctx: %d locale: %d\n", sr.ctx_id, sr.locale_index);
}

static void print_modfrag_code(modfrag_t frag)
{
	printf("		instructions %d immediates %d\n", frag.frag.code.insts, frag.frag.code.imms);
}

static void print_modfrag_data(modfrag_t frag)
{
	printf("		words %d\n", frag.frag.data.words);
}



void print_module(module_t *module)
{
	printf("module %d type:%d\n", module->tag, module->type);
	for(int i = 0; i < module->size; ++i)
	{
		modfrag_t frag = module->fragments[i];

		print_scope_ref(frag.ref);
		switch(module->type)
		{
			case MODULE_CODE:
				print_modfrag_code(frag);
				break;
			case MODULE_DATA:
				print_modfrag_data(frag);
				break;
			default:
				printf("unknown type %d\n", module->type);
				break;
		}
	}
}

scope_t *get_scope_from_ref(struct linker *lk, scope_ref_t ref)
{
	if(ref.ctx_id < 0 || ref.locale_index < 0)
		return NULL;
	if(ref.ctx_id >= get_number_of_sources())
		return NULL;
	context_t *ctx =  lk->srcs[ref.ctx_id].ctx;

	if(ctx->locales.size > ref.locale_index && ref.locale_index >= 0)
	{
		scope_t *scope = get_scope_from_context(ctx, ref.locale_index);
		return scope;
	}
	else
		return NULL;

}


void fill_module(struct linker *lk, module_t *mod)
{


	for(int fi = 0; fi < mod->size; ++fi)
	{
		fill_fragment(lk, mod, fi);


	}
	mod->filled = true;
}

//this is pretty shit
static size_t total_module_size_code(module_t *mod)
{

	size_t insts = 0;
	size_t imms = 0;


	for(int fi = 0; fi < mod->size; ++fi)
	{
		modfrag_t frag = mod->fragments[fi];
		imms += frag.frag.code.imms;
		insts += frag.frag.code.insts;
	}

	size_t actual_insts = (insts / 2) + (insts % 2);
	const size_t desc = 6;

	size_t table = (imms / 128) + 1;

	return actual_insts + desc + table + imms;
}

//this is pretty shit
static size_t total_module_size_data(module_t *mod)
{
	size_t words = 0;

	for(int fi = 0; fi < mod->size; ++fi)
	{
		modfrag_t frag = mod->fragments[fi];
		words += frag.frag.data.words;
	}
	return words;
}

size_t total_module_size(module_t *mod)
{
	if(mod->filled == false)
	{
		printf("to find total size you must have a filled module\n");
		return 0;
	}

	switch(mod->type)
	{

		case MODULE_CODE:
			return total_module_size_code(mod);
		case MODULE_DATA:
			return total_module_size_data(mod);
		default:
			printf("module type is invalid somehow %d\n", mod->type);
			exit(1);
	}

	return 0;
}

context_t *get_context_from_ref(struct linker *lk, scope_ref_t ref)
{
	return lk->srcs[ref.ctx_id].ctx;
}