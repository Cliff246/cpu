#ifndef __WCPU_PART_LEDGER_OPERATION_HEADER__
#define __WCPU_PART_LEDGER_OPERATION_HEADER__

#include "ledger.h"
#include "ledger_tensor.h"

#include <stdint.h>
#include <stdbool.h>

#define LEDGER_OPERATION_QUERY_SIZE 32

typedef enum ledger_operation_type
{
	LEDGER_OPERATION_INVAL,
	LEDGER_OPERATION_MARKOFF,
	LEDGER_OPERATION_MARK,
	LEDGER_OPERATION_QUERY,
	LEDGER_OPERATION_FLUSH,
}ledger_operation_type_t;


typedef struct ledger_operation_mark
{
	char dst[WCPU_LEDGER_ENTRY_ID_SIZE];
	char rs1[WCPU_LEDGER_ENTRY_ID_SIZE];

	char rs2[WCPU_LEDGER_ENTRY_ID_SIZE];

	char rs3[WCPU_LEDGER_ENTRY_ID_SIZE];
	bool uses_acc;
	int order;
	int edge;

}ledger_operation_mark_t;

typedef struct ledger_operation_markoff
{
	char markoff[WCPU_LEDGER_ENTRY_ID_SIZE];
	int epoch;
	int edge;
}ledger_operation_markoff_t;

typedef struct ledger_operation_query
{
	char query[LEDGER_OPERATION_QUERY_SIZE];
}ledger_operation_query_t;

typedef struct ledger_operation
{
	ledger_operation_type_t type;

	union
	{
		ledger_operation_markoff_t markoff;
		ledger_operation_mark_t mark;
		ledger_operation_query_t query;
	}op;

}ledger_operation_t;

#endif