#ifndef __WCPU_PART_LEDGER_HEADER__
#define __WCPU_PART_LEDGER_HEADER__

#define LEDGER_DEPENDENCIES_LIMIT 128
#define LEDGER_EPOCH_LIMIT 128
#define LEDGER_EDGE_LIMIT 4

#include "wcpu_part.h"
#include "wcpu_part_signal.h"

#include "ledger_operation.h"
#include "ledger_tensor.h"

#include <stdbool.h>


typedef struct wcpu_part_ledger
{
	ledger_tensor_t tensor;
}ledger_t;

part_ptr_t wcpu_ledger_generate(void);
void wcpu_ledger_update(part_t *part);

bool wcpu_ledger_import(part_t *part, part_signal_t *signal);
bool wcpu_ledger_export(part_t *part, part_signal_t **signal);

#endif