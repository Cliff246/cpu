#ifndef __LEDGER_HEADER__
#define __LEDGER_HEADER__

#define LEDGER_DEPENDENCIES_LIMIT 128
#define LEDGER_EPOCH_LIMIT 128
#define LEDGER_EDGE_LIMIT 4


typedef struct ledger_entry
{
	char fabid;

}ledger_entry_t;

typedef struct ledger
{
	int current_row;
	ledger_entry_t entries[LEDGER_DEPENDENCIES_LIMIT][LEDGER_EPOCH_LIMIT][LEDGER_EDGE_LIMIT];

}ledger_t;

ledger_t *create_ledger();



#endif