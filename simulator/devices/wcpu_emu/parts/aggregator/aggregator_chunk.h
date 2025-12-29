#ifndef __WCPU_PART_AGGREAGTOR_CHUNK_HEADER__
#define __WCPU_PART_AGGREAGTOR_CHUNK_HEADER__

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define WCPU_AGGREGATOR_CHUNK_CONTENT_SIZE 8

typedef enum wcpu_aggregator_chunk_type
{
	AGGREGATOR_CHUNK_EMPTY,
	AGGREGATOR_CHUNK_INSTRUCTIONS,
	AGGREGATOR_CHUNK_IMMEDIATES,
}aggregator_chunk_type_t;

typedef struct wcpu_aggregator_chunk_empty
{

}aggregator_chunk_empty_t;


typedef struct wcpu_aggregator_chunk_instructions
{
	uint64_t pc_start;

	uint64_t used_start;
	uint64_t used_offset;
	bool paired;
}aggregator_chunk_instructions_t;

typedef struct wcpu_aggregator_chunk_immediates
{
	uint64_t ipc_start;
	uint64_t pairs;
}aggregator_chunk_immediates_t;

typedef struct wcpu_aggregator_chunk
{
	aggregator_chunk_type_t type;
	int64_t buffer[WCPU_AGGREGATOR_CHUNK_CONTENT_SIZE];
	int ref;
	int last_changed;
	int last_used;
	int edge;
	union
	{
		aggregator_chunk_empty_t empty;
		aggregator_chunk_instructions_t insts;
		aggregator_chunk_immediates_t imms;
	}chunk;
}aggregator_chunk_t;

#endif
