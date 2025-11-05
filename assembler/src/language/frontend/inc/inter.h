


#ifndef __INTER__
#define __INTER__

#include "hashmap.h"
#include "lexer.h"
#include "parser.h"
#include <stdbool.h>
#include "strtools.h"
#include "decoder.h"
#include "fileio.h"

#include "instruction.h"
#include "metaop.h"
#include "entry.h"
#include "scope.h"
#include "symbol.h"
#include "directive.h"




typedef struct previous_contexts
{
	lexer_ctx_t *l_ctx;
	parser_ctx_t *p_ctx;
}prev_ctx_t;


typedef enum local_type
{
	LOCAL_WRITABLE,
}local_type_t;

typedef struct local
{
	//order

	scope_t scope;
	//has been resolved into elements
	bool loaded;
	bool resolved;
	//can be emited
	bool emitable;
	bool passed;

}local_t;

struct directive;



//directives
typedef struct directive_record
{
	int *records;
	size_t alloc;
	size_t count;
}dir_rec_t;



//order of directive_record
#define DIRECTIVE_CONTEXT_RECORD_INVAL 	DIR_INVAL
#define DIRECTIVE_CONTEXT_RECORD_PUBLIC DIR_PUB
#define DIRECTIVE_CONTEXT_RECORD_IMPORT DIR_IMP
#define DIRECTIVE_CONTEXT_RECORD_DEFINE DIR_DEF
#define DIRECTIVE_CONTEXT_RECORD_ORDER  DIR_ORDER



typedef struct context_directives
{
	dir_rec_t records[DIRECTIVES_TYPES_COUNT];
	struct directive **directives;
	size_t count, alloc;

}ctx_dirs_t;



//tores the locals
typedef struct locales
{

	local_t *locales;
	size_t size;
}locale_t;


typedef struct alias
{
	symbol_t *symbol;
	int index;
}alias_t;




typedef struct context
{

	parse_node_t *head;

	prev_ctx_t staged_ctx;
	//holds the results of the ref_table ptr

	locale_t locales;


	alias_t *aliases;
	size_t alias_count, alias_alloc;

	p_hashtable_t alias_map;
	int desc_id;

	ctx_dirs_t dirs;


	bool resolved;

}context_t;



bool uses_symbol(context_t *ctx, char *key);
context_t *load_context(file_desc_t *desc);
void context_resolve(context_t *ctx);
local_t *create_local(context_t *ctx, scope_t scope);
char *get_filename_from_context(context_t *ctx);

void print_directives(context_t *ctx);
void print_imports(context_t *ctx);
void print_defines(context_t *ctx);
void print_publics(context_t *ctx);



scope_t *get_scope_from_context(context_t *ctx, int index);
size_t get_number_of_scope_from_context(context_t *ctx);

void add_local_to_locale(context_t *ctx, local_t local);

bool is_symbol_implemented(context_t *ctx, char *key);




#endif
