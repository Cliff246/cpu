#ifndef __WCPU_PART_AGGREGATOR_HEADER__
#define __WCPU_PART_AGGREGATOR_HEADER__

#include "wcpu_part.h"
#include <stdlib.h>
#include <stdint.h>

typedef struct merge_entry
{
	int pc;
	int32_t inst;
	int64_t imm;
}merge_entry_t;

#define AGGREGATOR_QUEUE_SIZE 8
#define AGGREGATOR_MAX_EDGES 4

typedef struct wcpu_part_aggregator
{
	merge_entry_t queue[AGGREGATOR_QUEUE_SIZE];

	int edges[AGGREGATOR_MAX_EDGES];


}aggregator_t;

part_ptr_t wcpu_aggregator_generate(void);



void wcpu_aggregator_update(part_t *aggregator);

#endif