#ifndef __SYMBOL__
#define __SYMBOL__

#include "reference.h"

typedef enum symtype
{
	SYMBOL_INVAL,
	SYMBOL_REFERENCE,
}symtype_t;




typedef struct symbol
{
	char *key;
	symtype_t type;
	union
	{
		ref_t *ref;

	}symbol;
}symbol_t;


symbol_t *create_symbol_with_ref(ref_t *ref);
symbol_t *create_symbol(void);
void free_symbol(symbol_t *sym);
void print_symbol(symbol_t *sym);


#endif