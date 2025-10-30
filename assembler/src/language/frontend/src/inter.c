#include "inter.h"
#include "commons.h"
#include <stdlib.h>
#include <string.h>
#include "arguments.h"
#include "entry.h"
#include "symbol.h"
#include "directive.h"



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




context_t *load_context(file_desc_t *desc)
{

	context_t *ctx = CALLOC(1, context_t);

	ctx->desc = desc;
	ctx->staged_ctx.l_ctx = create_token_stream(desc->src, desc->id);

	//printf("finished lexer\n");



	ctx->staged_ctx.p_ctx = create_context(ctx->staged_ctx.l_ctx);
	//printf("created parser\n");
	ctx->head = parse_program(ctx->staged_ctx.p_ctx);
	//printf("done parser\n");
	ctx->locales.size = 0;
	ctx->locales.locales = NULL;
	//print_depth(ctx->head, 0);

	//this is over kill likely
	const int max_size = 100;
	ctx->alias_alloc = max_size;
	ctx->aliases = CALLOC(ctx->alias_alloc, alias_t);
	ctx->alias_map = new_hash_table(1000, free_alias);

	ctx->dirs_alloc = max_size;
	ctx->directives = CALLOC(ctx->dirs_alloc, directive_t);

	ctx->publics_alloc = 1;
	ctx->publics_count = 0;
	ctx->publics = CALLOC(ctx->publics_alloc, int);

	ctx->imports_alloc = 1;
	ctx->imports_count = 0;
	ctx->imports = CALLOC(ctx->imports_alloc, int);

	ctx->defines_alloc = 1;
	ctx->defines_count = 0;
	ctx->defines = CALLOC(ctx->defines_alloc, int);



	ctx->resolved = false;
	return ctx;
}




void context_resolve(context_t *ctx)
{
	//current semgents
	for(int s = 0; s < ctx->head->child_count; ++s)
	{
		parse_node_t *child = ctx->head->children[s];
		//printf("child %s\n",child->tok->lexeme);
		if(child->kind == NODE_SEGMENT)
		{
			scope_t scope = create_scope(child);

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
				exit(1);
			}
			apply_directive(ctx, dir);
		//	printf("directive %s\n", dir->name);


		}



	}
	#define OVERLAPPING_MAXIMUM 256
	char *overlaping[OVERLAPPING_MAXIMUM] = {0};
	int overlap_current = 0;
	for(int d = 0; d < ctx->defines_count; ++d)
	{
		int define = ctx->defines[d];
		directive_t *dir = ctx->directives[define];
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
					LOG("too many overlaps in a single file %s\n", ctx->desc->name, 0);
					exit(1);
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
			LOG("overlapped defintion at file %s with key %s\n", ctx->desc->name, overlaping[ol]);
		}
		exit(EXIT_FAILURE);
	}

	ctx->resolved = true;
	//print_hash_table(ctx->alias_map);
	//print_directives(ctx);
	//print_publics(ctx);
}





void print_directives(context_t *ctx)
{
	for(int i = 0; i < ctx->dirs_count; ++i)
	{
		directive_t *dir = ctx->directives[i];

		char buffer[DEFAULT_BUFFER + 1] = {0};
		size_t checkpoint = 0;
		for(int i = 0; i < MAX_DIRECTIVE_CONTENTS; ++i)
		{

			char *cur = dir->contents[i].content;

			if(cur == NULL || dir->contents[i].type != DIRARG_UNDEFINED)
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

		LOG("content: %s type:%d index %d\n", buffer, dir->type, dir->index, 0);
	}
}

void print_imports(context_t *ctx)
{
	for(int i = 0; i < ctx->imports_count; ++i)
	{
		int import = ctx->imports[i];
		directive_t *dir = ctx->directives[import];

		char buffer[DEFAULT_BUFFER + 1] = {0};
		size_t checkpoint = 0;
		for(int i = 0; i < MAX_DIRECTIVE_CONTENTS; ++i)
		{

			char *cur = dir->contents[i].content;
			if(cur == NULL || dir->contents[i].type != DIRARG_UNDEFINED)
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
	for(int i = 0; i < ctx->defines_count; ++i)
	{
		int define = ctx->defines[i];
		directive_t *dir = ctx->directives[define];
		char buffer[DEFAULT_BUFFER + 1] = {0};
		size_t checkpoint = 0;
		for(int i = 0; i < MAX_DIRECTIVE_CONTENTS; ++i)
		{
			char *cur = dir->contents[i].content;
			if(cur == NULL || dir->contents[i].type != DIRARG_UNDEFINED)
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
	for(int i = 0; i < ctx->publics_count; ++i)
	{
		int public = ctx->publics[i];
		directive_t *dir = ctx->directives[public];
		char buffer[DEFAULT_BUFFER + 1] = {0};
		size_t checkpoint = 0;
		for(int i = 0; i < MAX_DIRECTIVE_CONTENTS; ++i)
		{
			char *cur = dir->contents[i].content;
			if(cur == NULL || dir->contents[i].type != DIRARG_UNDEFINED)
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