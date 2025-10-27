#include "inter.h"
#include "commons.h"
#include <stdlib.h>
#include <string.h>
#include "arguments.h"
#include "entry.h"
#include "symbol.h"




void free_alias(void *ptr)
{
	alias_t *ali = (alias_t *)ptr;
	 
}

alias_t *create_alias(context_t *ctx, symbol_t *sym)
{
	if(ctx->aliases <= 0)
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
	ctx->alias_map = new_hash_table(max_size, free_alias);

	return ctx;
}




void context_resolve(context_t *ctx)
{
	//current semgents
	for(int s = 0; s < ctx->head->child_count; ++s)
	{
		parse_node_t *child = ctx->head->children[s];
		printf("child %s\n",child->tok->lexeme);
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



	}
	print_hash_table(ctx->alias_map);
}

