#ifndef __LINKER_HEADER__
#define __LINKER_HEADER__





#include "inter.h"
#include "commons.h"
#include "hashmap.h"
#include <stdlib.h>
#include "segment.h"
#include "module.h"

#define LINKER_MAX_FILES ASM_MAX_FILES
#define HASHTABLE_LINKER_GLOBALS 1000
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

typedef struct linker_source
{
	context_t *ctx;



}linker_src_t;



struct module;
//very screwed

typedef struct linker_tagorder
{
	char *tag;
	bool exists;
}linker_tagorder_t;

typedef struct linkorder
{
	//tag order parts
	linker_tagorder_t *tagorder;
	size_t count;
}linkorder_t;


typedef struct linker
{
	linker_src_t srcs[LINKER_MAX_FILES];

	global_t *global_store;
	size_t global_alloc, global_count;
	p_hashtable_t globals;
	ctx_inc_mat_t matrix;
	struct module modules[MAX_TAGS];
	linkorder_t order;
}linker_t;


void free_linker(linker_t *lk);

void add_context_to_linker(linker_t *lk, context_t *context);
linker_t *create_linker(void);
void print_globals(linker_t *lk);

bool check_global_validity(linker_t *lk);
size_t get_number_of_sources(void);
void build_module_stack(linker_t *lk);

context_t *get_context_from_global(linker_t *lk, global_t *glb);

symbol_t *get_symbol_from_global(linker_t *lk, global_t *glb);
bool add_tag_to_tagorder(linker_t *lk, char *key, int order);
void print_linker_tagorder(linker_t *lk);
int create_link_order(linker_t *lk, int *buffer, int bufsize);

#endif