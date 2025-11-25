#include "cache.h"
#include <stdlib.h>

part_ptr_t wcpu_cache_generate(void)
{

	cache_t *cache = calloc(1, sizeof(cache_t));


	part_ptr_t pptr;
	pptr.cache = cache;
	return pptr;
}

void wcpu_cache_update(part_t *cache)
{

}


