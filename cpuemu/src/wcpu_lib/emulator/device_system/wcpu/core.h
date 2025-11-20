#ifndef __CORE_HEADER__
#define __CORE_HEADER__

#include "wcpu_part.h"


#define CORE_PARTS_LIST(X)				\
	X(REGFILE 		, 1)				\
	X(LSU     		, 1)				\
	X(CACHE   		, 1)				\
	X(AGGREGATOR	, 1)				\
	X(MATTRESS		, 1)				\
	X(FETCHER		, 1)				\
	X(LEDGER 		, 1)				\
	X(ALU 			, 1)				\
	X(ALU 			, 1)				\

#define CORE_PARTS_ADD(x, y) + (1 * y)
#define COUNT_CORE_PARTS (0 CORE_PARTS_LIST(CORE_PARTS_ADD))

typedef enum core_part_id
{
	CORE_PART_REGFILE,
	CORE_PART_LSU,
	CORE_PART_CACHE,
	CORE_PART_AGGREGATOR,
	CORE_PART_FETCHER,
	CORE_PART_MATTRESS,
}core_part_id_t;


typedef struct
{
	part_t *parts[COUNT_CORE_PARTS];

}core_t;

core_t *wcpu_core_generate(void);

void wcpu_core_update(void *);

#endif