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

#define CORE_PARTS_ADD(x, y) + (1 * y)
#define COUNT_CORE_PARTS (0 CORE_PARTS_LIST(CORE_PARTS_ADD))

#define CORE_PART_NAME(X, Y) WCPU_PART_ ## X
#define CORE_PART_NAME_ENUM(X, Y) CORE_PART_NAME(X, Y),




#define CORE_PART_ORDER(X, Y) [CORE_PART_NAME(X, 0)] = Y,

extern int parts_order[];

typedef struct
{
	part_t *parts[COUNT_CORE_PARTS];

}core_t;

static core_t *wcpu_create_core(void);

core_t *wcpu_core_generate(void);
void wcpu_core_update(core_t *core);

void wcpu_core_handle_messages(core_t *core);

#endif