#ifndef __LINKER_HEADER__
#define __LINKER_HEADER__

#include "inter.h"
#include "commons.h"
#include "hashmap.h"
#include <stdlib.h>


#define HASHTABLE_LINKER_GLOBALS 1000
#define LINKER_MAX_FILES 8
#define LINKER_FILE_BITS  (LINKER_MAX_FILES / __CHAR_BIT__) + 1

typedef enum global_type
{
	GLOBAL_INVAL,
	GLOBAL_SYMBOL,
	GLOBAL_IMPORT
}global_type_t;

typedef struct global_source
{
	unsigned char files[LINKER_FILE_BITS];

}global_src_t;

typedef struct global_implementation
{
	int ctx_index;
	bool implemented;

}global_impl_t;

typedef struct global_import
{
	int ctx_source;
	int ctx_target;
	bool valid;
}global_import_t;

typedef struct global_symbol
{
	global_src_t srcs;
	global_impl_t imps;
}global_symbol_t;

typedef struct global
{

	union
	{
		global_symbol_t symbol;
		global_import_t import;
	}glb;
	global_type_t type;
	char *key;
	bool used;

}global_t;

void free_global(void *ptr);


typedef struct context_include_matrix
{
	char mat[LINKER_MAX_FILES][LINKER_MAX_FILES];
}ctx_inc_mat_t;

typedef struct linker
{
	context_t *ctx[LINKER_MAX_FILES];
	size_t ctx_size;

	global_t *global_store;
	size_t global_alloc, global_count;
	p_hashtable_t globals;
	ctx_inc_mat_t matrix;

}linker_t;


void add_context_to_linker(linker_t *lk, context_t *context);
linker_t *create_linker(void);
void print_globals(linker_t *lk);
char *get_filename_from_code(linker_t *lk, int index);

bool check_global_validity(linker_t *lk);


#endif