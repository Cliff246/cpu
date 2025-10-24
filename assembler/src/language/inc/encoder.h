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

typedef struct segout_extern
{
	//todo
}segout_extern_t;

typedef struct segout
{
	iseg_type_t type;

	union
	{
		segout_txt_t txt;
		segout_data_t data;
		segout_extern_t exteral;
	}output;
}segout_t;

typedef struct region
{
	size_t position;
	size_t size;
}region_t;

typedef struct
{
	region_t *regions;
	size_t regions_size;
}style_t;

segout_t create_segout(icontext_t *ctx, int segindex, style_t *style);

output_t *combine_segouts(segout_t *segouts, int length);


size_t resolve_addresses(icontext_t *ctx, style_t *style);
output_t *emit(icontext_t *ctx);

uint64_t resolve_size(iscope_t *scope);
void write_out(output_t *output, char *name);
#endif
