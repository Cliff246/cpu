#ifndef __AGGREGATOR_HEADER__
#define __AGGREGATOR_HEADER__

#include <stdlib.h>
#include <stdint.h>

typedef struct aggregate
{
	int pc;
	int32_t inst;
	int64_t imm;
}aggregate_t;

#define AGGREGATOR_QUEUE_SIZE 8
#define AGGREGATOR_MAX_EDGES 4

typedef struct aggregator
{
	aggregate_t queue[AGGREGATOR_QUEUE_SIZE];

	int edges[AGGREGATOR_MAX_EDGES];


}aggregator_t;

aggregator_t *create_aggregator(void);


void aggregator_step(aggregator_t *aggregator);

#endif