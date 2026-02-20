#ifndef __MOD_SYMMAP_HEADER__
#define __MOD_SYMMAP_HEADER__

#include "OBJ_hook.h"
#include <stdint.h>

typedef struct MOD_symbol
{
	char *key;
	OBJ_hook_t *hook;
}MOD_sym_t;

typedef struct MOD_symbol_map
{
	uint64_t size;
	MOD_sym_t symbols[];
}MOD_symmap_t;



#endif