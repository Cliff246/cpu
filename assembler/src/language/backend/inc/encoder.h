#ifndef __ENCODER__
#define __ENCODER__

#include "lexer.h"
#include "parser.h"
#include "inter.h"

typedef struct output
{
	uint64_t *bin;
	uint64_t size;
}output_t;



uint64_t text_resolve(scope_t *txt);

uint64_t data_resolve(scope_t *data);

segout_data_t create_segout_data(context_t *ctx, int index);

output_t *combine_segouts(segout_t *segouts, int length);
segout_t create_segout(context_t *ctx, int segindex);

size_t resolve_addresses(context_t *ctx);

output_t *emit(context_t *ctx);

uint64_t resolve_size(scope_t *scope);
void write_out(output_t *output, char *name);
#endif
