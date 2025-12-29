#ifndef __WCPU_PART_AGGREGATOR_HEADER__
#define __WCPU_PART_AGGREGATOR_HEADER__

#include "wcpu_part.h"
#include "aggregator_chunk.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


#define WCPU_AGGREGATOR_CHUNKS_SIZE 32

#define WCPU_AGGREGATOR_CODE_TABLE_CACHE_SIZE 16

typedef struct wcpu_aggregator_ct_elem
{
	uint64_t address;
	uint64_t value;
	int time;
	int cd_base;
}aggregator_ct_elem_t;


typedef struct wcpu_part_aggregator
{

	aggregator_chunk_t chunks[WCPU_AGGREGATOR_CHUNKS_SIZE];
	aggregator_ct_elem_t ct_cache[WCPU_AGGREGATOR_CODE_TABLE_CACHE_SIZE];

	

}aggregator_t;

part_ptr_t wcpu_aggregator_generate(void);



void wcpu_aggregator_update(part_t *part);

bool wcpu_aggregator_import(part_t *part, part_signal_t *signal);
bool wcpu_aggregator_export(part_t *part, part_signal_t **signal);

#endif