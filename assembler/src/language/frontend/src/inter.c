#include "inter.h"
#include "commons.h"

#include "arguments.h"
#include "entry.h"
#include "symbol.h"
#include "directive.h"
#include "eerror.h"
#include <stdlib.h>
#include <string.h>


void free_alias(void *ptr)
{
	alias_t *ali = (alias_t *)ptr;

}

bool uses_symbol(context_t *ctx, char *key)
{
	bool uses = false;
	for(int i = 0; i < ctx->locales.size; ++i)
	{
	   if(scope_uses_symbol(&ctx->locales.locales[i].scope, key))
	   		uses = true;
	}
	return uses;
}

alias_t *create_alias(context_t *ctx, symbol_t *sym)
{
	if(ctx->alias_alloc <= 0)
	{
		ctx->aliases = CALLOC((ctx->alias_alloc = 100), alias_t );

	}
	else if(ctx->alias_count >= ctx->alias_alloc)
	{
		ctx->aliases = REALLOC(ctx->aliases,(ctx->alias_alloc *= 2), alias_t );

	}

	ctx->aliases[ctx->alias_count].index = ctx->alias_count;
	ctx->aliases[ctx->alias_count].symbol = sym;
	if(getdata_from_hash_table(ctx->alias_map, sym->key))
	{
		perror("cannot duplicate reference key");

	}

	addto_hash_table(ctx->alias_map, sym->key, ctx->aliases + ctx->alias_count);
	return ctx->aliases + ctx->alias_count++;
}


local_t *create_local(context_t *ctx, scope_t scope)
{
	local_t local = {0};
	if(ctx->locales.size == 0)
	{
		ctx->locales.locales = CALLOC(1, local_t);
	}
	else
	{
		ctx->locales.locales = REALLOC(ctx->locales.locales, ctx->locales.size + 1, local_t);

	}
	local.resolved = false;
	local.scope = scope;
	local.loaded = true;
	local.passed = true;
	ctx->locales.locales[ctx->locales.size++] = local;
	return ctx->locales.locales + ctx->locales.size;
}

char *get_filename_from_context(context_t *ctx)
{
	return get_path_from_identifier(ctx->desc_id);
}



context_t *load_context(file_desc_t *desc)
{

	context_t *ctx = CALLOC(1, context_t);

	ctx->desc_id = desc->id;
	ctx->staged_ctx.l_ctx = create_token_stream(desc->src, desc->id);

	LOG("finished lexer\n", 0);



	ctx->staged_ctx.p_ctx = create_context(ctx->staged_ctx.l_ctx);

	LOG("created parser\n", 0);
	ctx->head = parse_program(ctx->staged_ctx.p_ctx);
	if(ctx->staged_ctx.p_ctx->error == true)
	{
		ctx->has_error = true;
		return ctx;
	}
	ctx->locales.size = 0;
	ctx->locales.locales = NULL;
	//print_depth(ctx->head, 0);

	//this is over kill likely
	const int max_size = 100;
	ctx->alias_alloc = max_size;
	ctx->aliases = CALLOC(ctx->alias_alloc, alias_t);
	ctx->alias_map = new_hash_table(1000, free_alias);



	ctx_dirs_t *ctx_dirs = &ctx->dirs;

	ctx_dirs->alloc = max_size;
	ctx_dirs->count = 0;
	ctx_dirs->directives =  CALLOC(ctx_dirs->alloc, directive_t);


	for(int record = 0; record < DIRECTIVES_TYPES_COUNT; record++)
	{
		dir_rec_t *drec = & ctx_dirs->records[record];
		drec->alloc = 1;
		drec->count = 0;
		drec->records = CALLOC(drec->alloc, int);
	}




	ctx->resolved = false;
	return ctx;
}




void context_resolve(context_t *ctx)
{
	if(ctx->has_error)
	{
		return;
	}
	//current semgents
	for(int s = 0; s < ctx->head->child_count; ++s)
	{
		parse_node_t *child = ctx->head->children[s];
		//printf("child %s\n",child->tok->lexeme);
		if(child->kind == NODE_SCOPE)
		{
			scope_t scope = create_scope(child);
			if(scope.errors > 0)
			{
				ctx->has_error = true;
			}
			local_t *local = create_local(ctx, scope);
			for(int i = 0; i < scope.symbols.count; ++i)
			{
				//printf("refs: %d %s\n", i, scope.refs.refs[i]->ref_string);
				create_alias(ctx,scope.symbols.symbols[i]);
			}

		}
		else if(child->kind == NODE_DIRECTIVE)
		{

			directive_t *dir = create_directive(ctx, child);
			if(dir == NULL)
			{
				LOG("directive at %d %d is inval", child->tok->locale.file,  child->tok->locale.row, 0);
				escape(1);
			}
			apply_directive(ctx, dir);
			LOG("directive %s\n", dir->head->tok->lexeme);


		}



	}



	#define OVERLAPPING_MAXIMUM 256
	char *overlaping[OVERLAPPING_MAXIMUM] = {0};
	int overlap_current = 0;
	ctx_dirs_t *dirs = &ctx->dirs;
	dir_rec_t *defines = &dirs->records[DIRECTIVE_CONTEXT_RECORD_DEFINE];

	for(int d = 0; d < defines->count; ++d)
	{
		int define = defines->records[d];
		directive_t *dir = dirs->directives[define];
		for(int a = 0; a < MAX_DIRECTIVE_CONTENTS; a++)
		{
			dirarg_t arg = dir->contents[a];
			if(arg.type == DIRARG_INVAL || arg.type == DIRARG_UNDEFINED)
				continue;
			alias_t *alias =  (alias_t *)getdata_from_hash_table(ctx->alias_map, arg.content);
			if(alias != NULL)
			{
				if(overlap_current >= OVERLAPPING_MAXIMUM)
				{
					LOG("too many overlaps in a single file %s\n", get_filename_from_context(ctx), 0);
					escape(1);
				}
				overlaping[overlap_current++] = alias->symbol->key;
			}
		}

	}
	if(overlap_current > 0)
	{
		for(int ol = 0; ol < overlap_current; ol++)
		{
			if(overlaping[ol] == NULL)
				continue;
			LOG("overlapped defintion at file %s with key %s\n", get_filename_from_context(ctx), overlaping[ol]);
		}
		escape(EXIT_FAILURE);
	}

	ctx->resolved = true;
	print_depth(ctx->head, 0);
	//print_hash_table(ctx->alias_map);
	print_directives(ctx);
	print_publics(ctx);
	print_defines(ctx);
	#undef OVERLAPPING_MAXIMUM
}





void print_directives(context_t *ctx)
{
	ctx_dirs_t *ctx_dirs = &ctx->dirs;

	for(int i = 0; i < ctx_dirs->count; ++i)
	{
		directive_t *dir = ctx_dirs->directives[i];

		char buffer[DEFAULT_BUFFER + 1] = {0};
		size_t checkpoint = 0;
		for(int i = 0; i < MAX_DIRECTIVE_CONTENTS; ++i)
		{

			char *cur = dir->contents[i].content;
			if(cur == NULL || dir->contents[i].type == DIRARG_UNDEFINED)
			{
				break;
			}
			size_t size = strlen(cur);
			size_t delta = DEFAULT_BUFFER - checkpoint;
			if(delta < size)
			{
				break;
			}
			else
			{
				memcpy(buffer + checkpoint, cur, size);
				checkpoint += size ;
				if(checkpoint < DEFAULT_BUFFER)
				{
					buffer[checkpoint] = ' ';
					checkpoint++;
				}
			}
		}

		LOG("content: %s type:%d index %d\n", buffer, dir->type, dir->index, 0);
	}
}

void print_imports(context_t *ctx)
{
	ctx_dirs_t *ctx_dirs = &ctx->dirs;
	dir_rec_t *imports_record = &ctx_dirs->records[DIRECTIVE_CONTEXT_RECORD_IMPORT];

	for(int i = 0; i < imports_record->count; ++i)
	{
		int import = imports_record->records[i];
		directive_t *dir = ctx_dirs->directives[import];

		char buffer[DEFAULT_BUFFER + 1] = {0};
		size_t checkpoint = 0;
		for(int i = 0; i < MAX_DIRECTIVE_CONTENTS; ++i)
		{

			char *cur = dir->contents[i].content;
			if(cur == NULL || dir->contents[i].type == DIRARG_UNDEFINED)
				break;


			size_t size = strlen(cur);
			size_t delta = DEFAULT_BUFFER - checkpoint;
			if(delta < size)
			{
				break;
			}
			else
			{
				memcpy(buffer + checkpoint, cur, size);
				checkpoint += size ;
				if(checkpoint < DEFAULT_BUFFER)
				{
					buffer[checkpoint] = ' ';
					checkpoint++;
				}
			}
		}
		LOG("import: %d content:%s type:%d index %d\n", import , buffer, dir->type, dir->index, 0);

	}
}
void print_defines(context_t *ctx)
{
	ctx_dirs_t *ctx_dirs = &ctx->dirs;
	dir_rec_t *defines_record = &ctx_dirs->records[DIRECTIVE_CONTEXT_RECORD_DEFINE];
	for(int i = 0; i < defines_record->count; ++i)
	{
		int define = defines_record->records[i];
		directive_t *dir = ctx_dirs->directives[define];
		char buffer[DEFAULT_BUFFER + 1] = {0};
		size_t checkpoint = 0;
		for(int i = 0; i < MAX_DIRECTIVE_CONTENTS; ++i)
		{
			char *cur = dir->contents[i].content;
			if(cur == NULL || dir->contents[i].type == DIRARG_UNDEFINED)
				break;

			size_t size = strlen(cur);
			size_t delta = DEFAULT_BUFFER - checkpoint;
			if(delta < size)
			{
				break;
			}
			else
			{
				memcpy(buffer + checkpoint, cur, size);
				checkpoint += size ;
				if(checkpoint < DEFAULT_BUFFER)
				{
					buffer[checkpoint] = ' ';
					checkpoint++;
				}
			}
		}
		LOG("define: %d content:%s type:%d index %d\n", define, buffer, dir->type, dir->index, 0);

	}
}

void print_publics(context_t *ctx)
{
	ctx_dirs_t *ctx_dirs = &ctx->dirs;
	dir_rec_t *publics_record = &ctx_dirs->records[DIRECTIVE_CONTEXT_RECORD_PUBLIC];


	for(int i = 0; i < publics_record->count; ++i)
	{
		int public = publics_record->records[i];
		directive_t *dir = ctx_dirs->directives[public];
		char buffer[DEFAULT_BUFFER + 1] = {0};
		size_t checkpoint = 0;
		for(int i = 0; i < MAX_DIRECTIVE_CONTENTS; ++i)
		{
			char *cur = dir->contents[i].content;
			if(cur == NULL || dir->contents[i].type == DIRARG_UNDEFINED)
				break;


			size_t size = strlen(cur);
			size_t delta = DEFAULT_BUFFER - checkpoint;
			if(delta < size)
			{
				break;
			}
			else
			{
				memcpy(buffer + checkpoint, cur, size);
				checkpoint += size ;
				if(checkpoint < DEFAULT_BUFFER)
				{
					buffer[checkpoint] = ' ';
					checkpoint++;
				}
			}
		}
		LOG("public: %d content:%s type:%d index %d\n", public, buffer, dir->type, dir->index, 0);

	}
}


bool is_symbol_implemented(context_t *ctx, char *key)
{
	if(ctx->resolved == false)
	{
		return false;
	}

	alias_t *alias = (alias_t*)getdata_from_hash_table(ctx->alias_map, key);
	if(alias == NULL)
		return false;
	else
		return true;
}

scope_t *get_scope_from_context(context_t *ctx, int index)
{
	if(ctx->locales.size < index)
		return NULL;
	if(index < 0)
		return NULL;

	scope_t *scope = &ctx->locales.locales[index].scope;
	return scope;
}

size_t get_number_of_scope_from_context(context_t *ctx)
{
	return ctx->locales.size;
}