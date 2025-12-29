#ifndef __WCPU_PART_SCHEDULER_HEADER__
#define __WCPU_PART_SCHEDULER_HEADER__

#include <stdint.h>
#include <stdbool.h>
#include "wcpu_part_ptr.h"
#include "wcpu_part_signal.h"
#include "wcpu_isa.h"

typedef struct wcpu_scheduler_owner
{
	int fabid;
	int owns[WCPU_REGISTER_COUNT];
	
}scheduler_owner_t;

typedef struct wcpu_part_scheduler
{
	int edges_inflight;
	//should be equal to max fabrics
	scheduler_owner_t owners[4];

}scheduler_t;

part_ptr_t wcpu_scheduler_generate(void);
void wcpu_scheduler_update(part_t *part);
bool wcpu_scheduler_import(part_t *part, part_signal_t *signal);
bool wcpu_scheduler_export(part_t *part, part_signal_t **signal);

#endif