


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


typedef struct segout_txt
{
	int64_t *inst;
	int64_t *imm;
	//table is inst_len / 128 + 1
	uint64_t *table;

	size_t inst_len, imm_len;

	uint64_t desc[6];
}segout_txt_t;

typedef struct segout_data
{
	uint64_t *data;
	size_t data_size;
}segout_data_t;


typedef struct segout
{
	seg_type_t type;

	union
	{
		segout_txt_t txt;
		segout_data_t data;
	}output;
}segout_t;




typedef struct region
{
	size_t position;
	size_t size;
}region_t;

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
	region_t region;
	segout_t out;
	//has been resolved into elements
	bool loaded;
	bool resolved;
	//can be emited
	bool emitable;
	bool passed;

}local_t;


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



typedef struct
{

	parse_node_t *head;

	prev_ctx_t staged_ctx;
	//holds the results of the ref_table ptr

	locale_t locales;
	alias_t *aliases;
	size_t alias_count, alias_alloc;
	p_hashtable_t alias_map;
	file_desc_t *desc;

}context_t;


context_t *load_context(file_desc_t *desc);
void context_resolve(context_t *ctx);
local_t *create_local(context_t *ctx, scope_t scope);


void add_local_to_locale(context_t *ctx, local_t local);

#endif
