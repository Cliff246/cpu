#ifndef __LEDGER_HEADER__
#define __LEDGER_HEADER__

#define LEDGER_DEPENDENCIES_LIMIT 128
#define LEDGER_EPOCH_LIMIT 128
#define LEDGER_EDGE_LIMIT 4

#include "wcpu_part.h"



typedef struct ledger_entry
{
	char fabid;

}ledger_entry_t;

typedef struct ledger
{
	int current_row;
	ledger_entry_t entries[LEDGER_DEPENDENCIES_LIMIT][LEDGER_EPOCH_LIMIT][LEDGER_EDGE_LIMIT];

}ledger_t;

part_ptr_t wcpu_ledger_generate(void);
void wcpu_ledger_update(part_t *ledger);



#endif