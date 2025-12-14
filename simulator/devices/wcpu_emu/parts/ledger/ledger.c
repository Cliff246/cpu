
#include "ledger.h"
#include "wcpu_part.h"
#include <stdlib.h>
#include <assert.h>

part_ptr_t wcpu_ledger_generate(void)
{
	ledger_t *led = calloc(1, sizeof(ledger_t));
	assert(led);
	part_ptr_t pptr;
	pptr.ledger = led;
	return pptr;
}

void wcpu_ledger_update(part_t *part)
{
	assert(part);
	assert(part->type == WCPU_PART_LEDGER);
}

bool wcpu_ledger_import(part_t *part, part_signal_t *signal)
{
	assert(part);
	assert(part->type == WCPU_PART_LEDGER);
	return true;
}
bool wcpu_ledger_export(part_t *part, part_signal_t **signal)
{
	assert(part);
	assert(part->type == WCPU_PART_LEDGER);
	return false;
}