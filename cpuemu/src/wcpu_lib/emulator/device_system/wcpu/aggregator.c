#include "wcpu_part_ptr.h"
#include "aggregator.h"
#include <stdlib.h>

part_ptr_t wcpu_aggregator_generate(void)
{
	aggregator_t *aggregator = calloc(1, sizeof(aggregator_t));
	part_ptr_t pptr;
	pptr.aggregator = aggregator;
	return pptr;
}



void wcpu_aggregator_update(part_ptr_t ptr)
{

}