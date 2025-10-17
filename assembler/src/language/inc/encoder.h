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


typedef struct region
{
	size_t position;
	size_t size;
}region_t;





void resolve_addresses(icontext_t *ctx);

uint64_t resolve_size(iscope_t *scope);

#endif
