#ifndef __CACHE_HEADER__
#define __CACHE_HEADER__

#include "wcpu_part.h"

#include <stdint.h>

typedef struct cache_line
{
	char tag;
	int64_t data[4];
	char flags;

}cacheline_t;

typedef struct cache
{
	int width;
	cacheline_t *lines;
}cache_t;


part_ptr_t wcpu_cache_generate(void);
void wcpu_cache_update(part_ptr_t cache);


#endif