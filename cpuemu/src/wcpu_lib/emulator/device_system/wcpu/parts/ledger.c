
#include "ledger.h"
#include <stdlib.h>

part_ptr_t wcpu_ledger_generate(void)
{
	ledger_t *led = calloc(1, sizeof(ledger_t));

	part_ptr_t pptr;
	pptr.ledger = led;
	return pptr;
}

void wcpu_ledger_update(part_t *ledger)
{

}


void wcpu_ledger_import( part_t *part, part_signal_t *signal)
{

}
bool wcpu_ledger_export( part_t *part, part_signal_t **signal)
{
	return false;
}