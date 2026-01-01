#include "scheduler.h"
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>



part_ptr_t wcpu_scheduler_generate(void)
{
	scheduler_t *scheduler = calloc(1, sizeof(scheduler_t));
	assert(scheduler);
	part_ptr_t pptr;
	pptr.scheduler = scheduler;
	return pptr;
}


void wcpu_scheduler_update(part_t *part)
{

}


bool wcpu_scheduler_import(part_t *part, part_signal_t *signal)
{
	return false;

}

bool wcpu_scheduler_export(part_t *part, part_signal_t **signal)
{
	return false;

}