#include "wcpu_part_ptr.h"
#include "aggregator.h"
#include <stdlib.h>
#include <assert.h>

part_ptr_t wcpu_aggregator_generate(void)
{
	aggregator_t *aggregator = calloc(1, sizeof(aggregator_t));
	part_ptr_t pptr;
	pptr.aggregator = aggregator;
	return pptr;
}


void wcpu_aggregator_update(part_t *part)
{
	assert(part != NULL && "aggregator cannot be null");

	assert(part->type == WCPU_PART_AGGREGATOR && "aggregator update ptr must have type of aggreagator");


	aggregator_t *aggregator = part->ptr.aggregator;




}

void wcpu_aggregator_import( part_t *part, part_signal_t *signal)
{
	assert(part != NULL && "aggregator cannot be null");

	assert(part->type == WCPU_PART_AGGREGATOR && "aggregator update ptr must have type of aggreagator");


	aggregator_t *aggregator = part->ptr.aggregator;


}

bool wcpu_aggregator_export( part_t *part, part_signal_t **signal)
{
	return false;
}