#ifndef __WCPU_PART_LEDGER_TENSOR_HEADER__
#define __WCPU_PART_LEDGER_TENSOR_HEADER__

#include <stdbool.h>
#include <stdint.h>
#include "wcpu_isa.h"

#define WCPU_LEDGER_TENSOR_EPOCHS_COUNT 512
#define WCPU_LEDGER_TENSOR_EDGES_COUNT 4



//more complex to help with debugging
typedef struct wcpu_ledger_entry
{
	int fabric;


}ledger_entry_t;

typedef struct wcpu_ledger_epoch
{
	ledger_entry_t entries[WCPU_REGISTER_COUNT];
	int epoch;
}ledger_epoch_t;

typedef struct wcpu_ledger_edge
{
	ledger_epoch_t epochs[WCPU_LEDGER_TENSOR_EPOCHS_COUNT];

	int epoch_base;
	int epoch_peak;
	int split;
}ledger_edge_t;

typedef struct wcpu_ledger_tensor
{
	ledger_edge_t edges[WCPU_LEDGER_TENSOR_EDGES_COUNT];

	int epoch_current;
}ledger_tensor_t;

#endif