#ifndef __WCPU_PART_CACHE_HEADER__
#define __WCPU_PART_CACHE_HEADER__

#include "wcpu_part.h"
#include "cache_line.h"
#include <stdint.h>
#include <stdbool.h>

#define CACHCE_CACHELINES_COUNT 128



typedef struct wcpu_part_cache
{
	int width;
	cache_line_t lines[CACHCE_CACHELINES_COUNT];
}cache_t;


part_ptr_t wcpu_cache_generate(void);
void wcpu_cache_update(part_t *part);

bool wcpu_cache_import(part_t *part, part_signal_t *signal);
bool wcpu_cache_export(part_t *part, part_signal_t **signal);

#endif