#include "mattress.h"
#include "wcpu_part.h"
#include <stdlib.h>
#include <assert.h>


part_ptr_t wcpu_mattress_generate(void)
{
	mattress_t *mattress = calloc(1, sizeof(mattress_t));
	assert(mattress);
	part_ptr_t pptr;
	pptr.mattress = mattress;
	return pptr;
}

void wcpu_mattress_update(part_t *part)
{
	assert(part);
	assert(part->type == WCPU_PART_MATTRESS);
}

bool wcpu_mattress_import( part_t *part, part_signal_t *signal)
{
	assert(part);
	assert(part->type == WCPU_PART_MATTRESS);
	return true;

}

bool wcpu_mattress_export( part_t *part, part_signal_t **signal)
{
	assert(part);
	assert(part->type == WCPU_PART_MATTRESS);
	return false;
}