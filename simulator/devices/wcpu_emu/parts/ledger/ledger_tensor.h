#ifndef __WCPU_PART_LEDGER_TENSOR_HEADER__
#define __WCPU_PART_LEDGER_TENSOR_HEADER__

#define WCPU_LEDGER_TENSOR_ENTRY_COUNT 128
#define WCPU_LEDGER_TENSOR_EPOCHS_COUNT 128
#define WCPU_LEDGER_TENSOR_EDGES_COUNT 4

#define WCPU_LEDGER_ENTRY_ID_SIZE 8

//more complex to help with debugging
typedef struct wcpu_ledger_entry
{

	char id[WCPU_LEDGER_ENTRY_ID_SIZE];
	//
	int fabric;
	//
	int instruction;
}ledger_entry_t;

typedef struct wcpu_ledger_epoch
{
	ledger_entry_t entries[WCPU_LEDGER_TENSOR_ENTRY_COUNT];

}ledger_epoch_t;

typedef struct wcpu_ledger_edge
{
	ledger_epoch_t epochs[WCPU_LEDGER_TENSOR_EPOCHS_COUNT];

	int epoch_low;
	int epoch_high;
	int epoch_center;
}ledger_edge_t;

typedef struct wcpu_ledger_tensor
{
	ledger_edge_t edges[WCPU_LEDGER_TENSOR_EDGES_COUNT];
}ledger_tensor_t;

#endif