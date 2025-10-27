#include "symbol.h"
#include "reference.h"
#include "commons.h"
#include <stdlib.h>
#include <stdio.h>

symbol_t *create_symbol_with_ref(ref_t *ref)
{

	symbol_t *sym = CALLOC(1, symbol_t);
	sym->key = ref->ref_string;
	sym->symbol.ref = ref;
	sym->type = SYMBOL_REFERENCE;
	return sym;
}

void free_symbol(symbol_t *sym)
{
	if(sym->type == SYMBOL_REFERENCE)
	{
		free_reference(sym.symbol.ref);
	}
	free(sym);

}

symbol_t *create_symbol(void)
{
	symbol_t *sym = CALLOC(1, symbol_t);
	return sym;
}