#ifndef __MODULE_HEADER__
#define __MODULE_HEADER__

struct module;

#include <stdlib.h>
#include "scope.h"

typedef struct scope_ref
{
	int ctx_id;
	int locale_index;
	int seg_id;
}scope_ref_t;



typedef enum module_type
{
	MODULE_UNSET,
	MODULE_CODE,
	MODULE_DATA,
	MODULE_INVAL,
}module_type_t;

typedef struct module_fragment_code
{
	size_t insts;
	size_t imms;


}modfrag_code_t;

typedef struct module_fragment_data
{
	size_t words;
}modfrag_data_t;

typedef struct module_fragment
{
	scope_ref_t ref;
	union
	{
		modfrag_data_t data;
		modfrag_code_t code;
	}frag;
}modfrag_t;


typedef struct module
{
	int tag;
	int size;
	module_type_t type;
	modfrag_t *fragments;
	bool set;
	bool filled;
}module_t;

struct linker;

bool scope_is_of_correct_type(module_type_t type, scope_t *scope);

void append_scope_ref(struct linker *lk, module_t *mod, int ctx_id, int local_index);

void fill_fragment(struct linker *lk, module_t *in, int index);

void print_module(module_t *module);
module_t *get_module(struct linker *lk, int sid);

scope_t *get_scope_from_ref(struct linker *lk, scope_ref_t ref);

void fill_module(struct linker *lk, module_t *mod);

size_t total_module_size(module_t *mod);

#endif