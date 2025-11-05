#include "inter.h"

#include "parser.h"
#include "directive.h"
#include <string.h>
#include <stdbool.h>

typedef struct dirtemplate
{
	const char *key;
	void (*fn)(context_t * const ctx, directive_t * const d); // handler callback
	directive_type_t type;

}dirtemplate_t;

//get the
dir_rec_t *get_record_from_context(context_t *ctx, directive_type_t type)
{
	int index = -1;
	switch(type)
	{
		case DIR_DEF:
			index = DIRECTIVE_CONTEXT_RECORD_DEFINE;
			break;
		case DIR_PUB:
			index = DIRECTIVE_CONTEXT_RECORD_PUBLIC;
			break;
		case DIR_IMP:
			index = DIRECTIVE_CONTEXT_RECORD_IMPORT;
			break;
		case DIR_ORDER:
			index = DIRECTIVE_CONTEXT_RECORD_ORDER;
			break;

		default:

			LOG("type is invalid for record %d\n", type, 0);
			exit(1);
			break;
	}
	//LOG("directive type %d\n ", type, 0);
	ctx_dirs_t *dirs = &ctx->dirs;

	dir_rec_t * record = &dirs->records[index];
	return record;
}

static void realloc_directive_record(dir_rec_t *record)
{
	if(record->count >= record->alloc)
	{
		record->records = REALLOC(record->records, record->alloc *= 2, int);
	}
}

static void directive_generic_handler(context_t *ctx, directive_t *d)
{
	dir_rec_t *record = get_record_from_context(ctx, d->type);
	realloc_directive_record(record);
	record->records[record->count++] = d->index;
}



void directive_public_handler(context_t *ctx, directive_t *d)
{
	directive_generic_handler(ctx, d);
	// TODO: implement public directive handler
}



void directive_def_handler(context_t *ctx, directive_t *d)
{
	directive_generic_handler(ctx, d);


	// TODO: implement def directive handler
}

void directive_imp_handler(context_t *ctx, directive_t *d)
{
	directive_generic_handler(ctx, d);

	// TODO: implement inc directive handler
}




void directive_order_handler(context_t *ctx, directive_t *d)
{
	directive_generic_handler(ctx, d);

}

static dirtemplate_t builtins[] =
{
	[DIR_INVAL] = {"INVAL", NULL, DIR_INVAL},
	[DIR_PUB]	= {"pub", directive_public_handler, DIR_PUB},
	[DIR_DEF] 	= {"def", directive_def_handler, DIR_DEF},
	[DIR_IMP] 	= {"imp", directive_imp_handler, DIR_IMP},
	[DIR_ORDER] = {"order", directive_order_handler, DIR_ORDER},

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

	ctx_dirs_t *dirs = &ctx->dirs;
	if(dirs->count <= dirs->count)
	{
		dirs->directives = REALLOC(dirs->directives, dirs->alloc *= 2, directive_t *);
	}
	dir->index = dirs->count;
	dirs->directives[dirs->count++] = dir;
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

			case DIR_ORDER:
				return DIRARG_TAG;
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
	//printf("dirkey: %s\n", template.key);
	dirarg_type_t args[MAX_DIRECTIVE_CONTENTS] = {DIRARG_UNDEFINED};
	for(int next = 0; next < MAX_DIRECTIVE_CONTENTS; ++next)
	{

		if(next < head->child_count)
		{
			parse_node_t *child = head->children[next];
			char *childtok = child->tok->lexeme;
			dir->contents[next].content = strdup(childtok);
			args[next] = fill_dirarg_type(template.type, args, next, child);
		}
		else
			args[next] = DIRARG_UNDEFINED;
		dir->contents[next].type = args[next];

	}

	dir->type = template.type;
	dir->head = head;
	dir->index = -1;
	//LOG("dirkey done\n", 0);
	return dir;
}
