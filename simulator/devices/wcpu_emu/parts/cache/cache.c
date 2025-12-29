#include "cache.h"
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

part_ptr_t wcpu_cache_generate(void)
{

	cache_t *cache = calloc(1, sizeof(cache_t));
	assert(cache);

	part_ptr_t pptr;
	pptr.cache = cache;
	return pptr;
}

void wcpu_cache_update(part_t *part)
{
	assert(part);
	assert(part->type == WCPU_PART_CACHE);
}


bool wcpu_cache_import(part_t *part, part_signal_t *signal)
{
	assert(part);
	assert(part->type == WCPU_PART_CACHE);
	return true;

}
bool wcpu_cache_export(part_t *part, part_signal_t **signal)
{
	assert(part);
	assert(part->type == WCPU_PART_CACHE);
	return false;
}
