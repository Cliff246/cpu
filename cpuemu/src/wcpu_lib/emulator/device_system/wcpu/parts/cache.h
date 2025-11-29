#ifndef __WCPU_PART_CACHE_HEADER__
#define __WCPU_PART_CACHE_HEADER__

#include "wcpu_part.h"
#include <stdint.h>
#include <stdbool.h>

#define CACHCE_CACHELINES_COUNT 128

typedef struct cache_line
{
	char tag;
	int64_t data[4];
	char flags;

}cacheline_t;

typedef struct wcpu_part_cache
{
	int width;
	cacheline_t lines[CACHCE_CACHELINES_COUNT];
}cache_t;


part_ptr_t wcpu_cache_generate(void);
void wcpu_cache_update(part_t *cache);

void wcpu_cache_import( part_t *part, part_signal_t *signal);
bool wcpu_cache_export( part_t *part, part_signal_t **signal);

#endif