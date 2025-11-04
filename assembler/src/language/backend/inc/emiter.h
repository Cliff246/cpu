#ifndef __EMITER_HEADER__
#define __EMITER_HEADER__

#include "linker.h"
#include "inter.h"
#include "lexer.h"

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
	module_type_t type;

	union
	{
		segout_txt_t txt;
		segout_data_t data;
	}output;
}segout_t;


typedef struct output
{
	uint64_t *bin;
	uint64_t size;
}output_t;



typedef struct region
{
	int order;
	size_t position;
	size_t size;
	module_t *mod;
	segout_t segout;
}region_t;

region_t *create_region(module_t *mod);

typedef struct outorder
{
	region_t **regions;

	size_t count;
}outorder_t;

outorder_t *create_outorder(linker_t *ll);

void set_outorder_positions(outorder_t *oo, int *positions, size_t size);

void resolve_positions(outorder_t *oo);
void fix_addresses(linker_t *ll, outorder_t *oo);
void emit_segouts(linker_t *ll, outorder_t *oo);
void print_over_outorder(linker_t *lk, outorder_t *oo);

segout_t create_segout(linker_t *ll, region_t *region);
void print_segout(segout_t *out);
output_t *combine_segouts(segout_t *segouts, int length);
void write_out(output_t *output, char *name);

#endif