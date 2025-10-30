
#include "linker.h"
#include "inter.h"
#include "commons.h"
#include "hashmap.h"
#include <stdbool.h>

void add_src_to_global(global_t *global, int index)
{

	global_symbol_t *glb = &global->glb.symbol;

	if(index < 0 || index > LINKER_MAX_FILES)
		return;
	int page = index / __CHAR_BIT__;
	int offset = index % __CHAR_BIT__;
	char content = glb->srcs.files[page];
	
	content = SETBIT(content, offset);
	glb->srcs.files[page] = content;
}

bool get_src_in_global(global_t *global, int index)
{
	if(index < 0 || index > LINKER_MAX_FILES)
		return false;
	global_symbol_t glb = global->glb.symbol;

	int page = index / __CHAR_BIT__;
	int offset = index % __CHAR_BIT__;
	char content = glb.srcs.files[page];
	int bit = GETBIT(content, offset);
	return bit;
}


void print_global_content(global_t *global)
{
	switch(global->type == GLOBAL_SYMBOL)
	{
		global_symbol_t glb = global->glb.symbol;

		printf("global %s srcs:\n", global->key);
		for(int i = 0; i < LINKER_FILE_BITS; ++i)
		{
			print_bin((char)glb.srcs.files[i], 8, false);

		}
		printf("\nimplemented %d file:%d\n", glb.imps.implemented, glb.imps.ctx_index);
	}


}

global_type_t get_global_type_from_directive(directive_type_t dirtype)
{
	switch(dirtype)
	{
		case DIR_DEF:
			return GLOBAL_SYMBOL;
		case DIR_IMP:
			return GLOBAL_IMPORT;
		case DIR_PUB:
			return GLOBAL_SYMBOL;
		case DIR_INVAL:
			return GLOBAL_INVAL;
		default:
			return GLOBAL_INVAL;
	}
}


global_t *add_global(linker_t *lk)
{
	if(lk->global_count >= lk->global_alloc)
	{
		lk->global_store = REALLOC(lk->global_store, lk->global_alloc *= 2, global_t);
	}

	global_t *ptr = lk->global_store + lk->global_count;
	ptr->used = true;
	lk->global_count++;
	return ptr;
}

global_t *get_global(linker_t *lk, int index)
{
	if(index < 0 || index >= lk->global_count)
	{
		return NULL;
	}

	else
	{
		return lk->global_store + index;
	}
}

int symbol_global_has_src(global_t *global)
{
	if(global->used == false)
		return 0;

	if(global->type != GLOBAL_SYMBOL)
		return 0;


	int count = 0;
	for(int i = 0; i < LINKER_FILE_BITS; ++i)
	{
		if(global->glb.symbol.srcs.files[i] != 0)
			count++;

	}
	return count;
}

bool global_matches(global_t *global)
{
	if(global->used == false)
		return false;
	if(global->type == GLOBAL_SYMBOL)
	{
		int srcs = symbol_global_has_src(global);
		if(global->glb.symbol.imps.implemented == false)
		{
			if(srcs > 0)
				return false;
			else

				return true;
		}
		if(srcs > 0)
			return true;
		return true;
	}
	else
	{
		return false;
	}

}

static int context_in_linker(linker_t *lk, char *filename)
{
	size_t len = strlen(filename);
	if(len > DEFAULT_BUFFER)
		return -1;
	char buffer[DEFAULT_BUFFER + 1] = {0};

	int quotes = remove_quotes(filename, buffer, DEFAULT_BUFFER);
	if(lk->ctx_size == 0)
		return -1;
	//if(!validate_path(filename))
	//return -1;

	for(int i = 0; i < lk->ctx_size; ++i)
	{
		context_t *ctx = lk->ctx[i];
		file_desc_t *desc = ctx->desc;
		LOG("descname: %s fname: %s\n", desc->name, buffer, 0);
		if(!strcmp(desc->name, buffer))
		{
			return i;
		}
	}
	return -1;

}


static void fill_global_via_directive(linker_t *lk, context_t *ctx, int index_dir)
{


	directive_t *dir = ctx->directives[index_dir];
	if(dir->type == DIR_IMP)
	{


		for(int da = 0; da < MAX_DIRECTIVE_CONTENTS; ++da)
		{
			dirarg_t arg = dir->contents[da];

			//requires context to be in scope

			if(arg.type == DIRARG_INVAL || arg.type == DIRARG_UNDEFINED)
			{
				continue;
			}
			int index = context_in_linker(lk, arg.content);
			if(index == -1)
			{
				LOG("%s include not in assembler context\n", arg.content, 0);
				exit(1);
			}
			global_t *new = add_global(lk);
			new->used = true;
			new->key = arg.content;
			new->type = GLOBAL_IMPORT;
			new->glb.import.ctx_source = ctx->desc->id;
			new->glb.import.ctx_target = index;
			new->glb.import.valid = true;

			lk->matrix.mat[ctx->desc->id][index] = 1;

		}
		return;
	}
	else
	{
		//da = dir arg
		for(int da = 0; da < MAX_DIRECTIVE_CONTENTS; ++da)
		{
			dirarg_t arg = dir->contents[da];
			if(arg.type == DIRARG_INVAL || arg.type == DIRARG_UNDEFINED)
			{
				continue;
			}
			global_t *glb = getdata_from_hash_table(lk->globals, arg.content);
			if(glb == NULL)
			{
				global_t *new = add_global(lk);
				new->key = arg.content;
				new->type = get_global_type_from_directive(dir->type);

				addto_hash_table(lk->globals, new->key, new);
				glb = new;


			}

			if(glb->type != get_global_type_from_directive(dir->type))
			{
				LOG("global type didnt match a directive type %d %d\n", glb->type, dir->type, 0);
				exit(1);
			}
			global_symbol_t *sym = &glb->glb.symbol;

			if(dir->type == DIR_PUB)
			{

				if(sym->imps.implemented == false)
				{
					sym->imps.ctx_index = ctx->desc->id;
					sym->imps.implemented = true;
				}
				else
				{
					LOG("%s already implmented in %s\n", glb->key, get_filename_from_code(lk, sym->imps.ctx_index), 0);
					exit(1);
				}
			}
			else if(dir->type == DIR_DEF)
			{

				if(!is_symbol_implemented(ctx, glb->key))
				{
					add_src_to_global(glb, ctx->desc->id);
				}
				else
				{
					LOG("global %s is already already defined in file %s\n", glb->key, get_filename_from_code(lk, sym->imps.ctx_index), 0);

				}
				//slow function
				//if(uses_symbol(ctx, glb->key))
				{
					//
				}
				//else
				{
				//	printf("unused define %s\n", glb->key);
				}

			}
			print_global_content(glb);

		}
		return;
	}

}

void add_context_to_linker(linker_t *lk, context_t *ctx)
{
	if(lk->ctx_size >= LINKER_MAX_FILES)
	{
		LOG("too many files added size:%d max_files%d\n",lk->ctx_size, LINKER_MAX_FILES, 0);
		exit(1);
	}

	lk->ctx[lk->ctx_size] = ctx;
	for(int i = 0; i < ctx->dirs_count; ++i)
	{
		fill_global_via_directive(lk, ctx, i);
	}



	//should grab included files and determine in scope names
	{


	}


	lk->ctx_size++;

	return;
}

void free_global(void *ptr)
{
	global_t *gptr = (global_t *)ptr;
	gptr->used = false;
}

char *get_filename_from_code(linker_t *lk, int index)
{
	return lk->ctx[index]->desc->name;
	return NULL;
}

linker_t *create_linker(void)
{
	linker_t *lk =  CALLOC(1, linker_t);


	lk->global_alloc = HASHTABLE_LINKER_GLOBALS;
	lk->global_count = 0;
	lk->global_store = CALLOC(lk->global_alloc, global_t);

	lk->globals = new_hash_table(HASHTABLE_LINKER_GLOBALS, free_global);
	for(int i = 0; i < LINKER_MAX_FILES; ++i)
	{
		lk->matrix.mat[i][i] = 1;

	}
	return lk;
}

void print_globals(linker_t *lk)
{
	for(int i = 0; i < lk->ctx_size; ++i)
	{
		context_t *ctx = lk->ctx[i];

		print_directives(ctx);

	}
}




bool check_global_validity(linker_t *lk)
{


	bool passes = true;
	for(int i = 0; i < lk->global_count; ++i)
	{
		global_t *glb = get_global(lk, i);
		if(glb->used == true)
		{
			if(glb->type == GLOBAL_IMPORT)
			{
				if(!glb->glb.import.valid)
				{
					passes = false;
				}
			}
			else if(!global_matches(glb))
			{
				printf("global key %s failed\n", glb->key);
				passes = false;
			}
		}

	}
	return passes;
}