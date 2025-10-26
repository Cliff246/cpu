#include "inter.h"
#include "commons.h"
#include <stdlib.h>
#include <string.h>
#include "arguments.h"
#include "entry.h"




void free_symbol(void *ptr)
{
	symbol_t *sym = (symbol_t *)ptr;
	free(sym);
}

symbol_t *create_symbol(context_t *ctx, ref_t *ref)
{
	symbol_t *sym = CALLOC(1, symbol_t);
	if(ctx->symbols_alloc <= 0)
	{
		ctx->symbols = CALLOC((ctx->symbols_alloc = 100), symbol_t *);

	}
	else if(ctx->symbols_count >= ctx->symbols_alloc)
	{
		ctx->symbols = REALLOC(ctx->symbols,(ctx->symbols_alloc *= 2), symbol_t *);

	}
	sym->index = ctx->symbols_count;
	sym->ref = ref;
	ctx->symbols[ctx->symbols_count].index = ctx->symbols_count;
	ctx->symbols[ctx->symbols_count].ref = ref;
	if(getdata_from_hash_table(ctx->symmap, sym->ref->ref_string))
	{
		perror("cannot duplicate reference key");

	}

	addto_hash_table(ctx->symmap, sym->ref->ref_string, ctx->symbols + ctx->symbols_count);
	ctx->symbols_count++;
	return ctx->symbols + ctx->symbols_count;
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

void print_local(local_t *l)
{
	printf("ref %d\n",l->scope.refs.count);
	for(int i = 0; i < l->scope.refs.count; ++i)
	{
		printf("locale ref:%s\n", l->scope.refs.refs[i]->ref_string);
	}
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
	print_depth(ctx->head, 0);

	//this is over kill likely
	const int max_size = 100;
	ctx->symbols_alloc = max_size;
	ctx->symbols = CALLOC(ctx->symbols_alloc, symbol_t *);
	ctx->symmap = new_hash_table(max_size, free_symbol);

	return ctx;
}




void context_resolve(context_t *ctx)
{
	//current semgents
	for(int s = 0; s < ctx->head->child_count; ++s)
	{
		parse_node_t *child = ctx->head->children[s];
		if(child->kind == NODE_SEGMENT)
		{
			scope_t scope = create_scope(child);

			local_t *local = create_local(ctx, scope);
			for(int i = 0; i < scope.refs.count; ++i)
			{
				create_symbol(ctx,scope.refs.refs[i]);
			}

		}



	}
	print_hash_table(ctx->symmap);
}

