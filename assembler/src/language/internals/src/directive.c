#include "parser.h"
#include "directive.h"
#include "inter.h"
#include <string.h>
#include <stdbool.h>

typedef struct dirtemplate
{
	const char *key;
	void (*fn)(context_t * const ctx, directive_t * const d); // handler callback
	directive_type_t type;

}dirtemplate_t;

void directive_public_handler(context_t *ctx, directive_t *d)
{
	if(ctx->publics_count <= ctx->publics_alloc)
	{
		ctx->publics = REALLOC(ctx->publics, ctx->publics_alloc *= 2, int);

	}


	ctx->publics[ctx->publics_count++] = d->index;

	// TODO: implement public directive handler
}

void directive_def_handler(context_t *ctx, directive_t *d)
{
	if(ctx->defines_count <= ctx->defines_alloc)
	{
		ctx->defines = REALLOC(ctx->defines, ctx->defines_alloc *= 2, int);

	}


	ctx->defines[ctx->defines_count++] = d->index;
	// TODO: implement def directive handler
}

void directive_imp_handler(context_t *ctx, directive_t *d)
{
	if(ctx->imports_count <= ctx->imports_alloc)
	{
		ctx->imports = REALLOC(ctx->imports, ctx->imports_alloc *= 2, int);

	}


	ctx->imports[ctx->imports_count++] = d->index;
	// TODO: implement inc directive handler
}

static dirtemplate_t builtins[] =
{
	[DIR_INVAL] = {"INVAL", NULL, DIR_INVAL},
	[DIR_PUB]	= {"pub", directive_public_handler, DIR_PUB},
	[DIR_DEF] 	= {"def", directive_def_handler, DIR_DEF},
	[DIR_IMP] 	= {"imp", directive_imp_handler, DIR_IMP},
};

char *inval = "INVAL";

// Looks up the directive keyword (after the %) and returns a template entry or a fallback invalid template.
dirtemplate_t get_dirtemplate(parse_node_t *head)
{
	if(head->kind != NODE_DIRECTIVE)
	{
		goto ERR;
	}
	char *fixed_lexme = head->tok->lexeme + 1;
	if(fixed_lexme[0] == 0)
	{
		goto ERR;
	}
	for(int i = 1; i < ARYSIZE(builtins); ++i)
	{
		dirtemplate_t *template = builtins + i;
		if(!strcmp(fixed_lexme, template->key))
		{
			return builtins[i];
		}

	}
	goto ERR;

ERR:

	return builtins[0];
}

void apply_directive(struct context *ctx, directive_t *dir)
{
	if(ctx->dirs_alloc <= ctx->dirs_count)
	{
		ctx->directives = REALLOC(ctx->directives, ctx->dirs_alloc *= 2, directive_t *);
	}
	dir->index = ctx->dirs_count;
	ctx->directives[ctx->dirs_count++] = dir;
	builtins[dir->type].fn(ctx, dir);

}

static dirarg_type_t fill_dirarg_type(directive_type_t type, dirarg_type_t args[MAX_DIRECTIVE_CONTENTS], int index, parse_node_t *cur)
{
	if(index >= MAX_DIRECTIVE_CONTENTS)
	{
		return DIRARG_INVAL;
	}
	if(index == 0)
	{
		switch(type)
		{
			case DIR_IMP:
				return DIRARG_FILE;
			case DIR_PUB:
				return DIRARG_REFERENCE;
			case DIR_DEF:
				return DIRARG_IMPLICIT;
			default:
				return DIRARG_INVAL;
		}
	}
	else
	{
		dirarg_type_t last = args[index - 1];


		return last;

	}

}

directive_t *create_directive(struct context *ctx, parse_node_t *head)
{
	if(head->kind != NODE_DIRECTIVE)
	{
		LOG("head is inval\n", 0);

		exit(1);
	}


	dirtemplate_t template = get_dirtemplate(head);
	if(template.type == DIR_INVAL)
	{
		LOG("directive is inval\n", 0);
		return NULL;
	}
	//printf("%s\n", template.key);
	directive_t *dir = CALLOC(1, directive_t);
	printf("dirkey: %s\n", template.key);
	dirarg_type_t args[MAX_DIRECTIVE_CONTENTS] = {DIRARG_UNDEFINED};
	for(int next = 0; next < MAX_DIRECTIVE_CONTENTS; ++next)
	{

		if(next < head->child_count)
		{
			parse_node_t *child = head->children[next];
			char *childtok = child->tok->lexeme;
			dir->contents[next].content = childtok;
			args[next] = fill_dirarg_type(template.type, args, next, child);
		}
		else
			args[next] = DIRARG_UNDEFINED;
		dir->contents[next].type = args[next];

	}

	dir->type = template.type;
	dir->head = head;
	dir->index = -1;

	return dir;
}
