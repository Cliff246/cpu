#ifndef __WCPU_PART_SCHEDULER_HEADER__
#define __WCPU_PART_SCHEDULER_HEADER__

#include <stdint.h>
#include <stdbool.h>
#include "wcpu_part_ptr.h"
#include "wcpu_part_signal.h"

typedef struct wcpu_part_scheduler
{

}scheduler_t;

part_ptr_t wcpu_scheduler_generate(void);
void wcpu_scheduler_update(part_t *part);
bool wcpu_scheduler_import(part_t *part, part_signal_t *signal);
bool wcpu_scheduler_export(part_t *part, part_signal_t **signal);

#endif