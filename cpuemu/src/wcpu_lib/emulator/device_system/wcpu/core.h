#ifndef __CORE_HEADER__
#define __CORE_HEADER__

#include "wcpu_part.h"


#define CORE_PARTS_LIST(X)	\
	X(INVALID)				\
	X(REGFILE)				\
	X(LSU)					\
	X(CACHE)				\
	X(AGGREGATOR)			\
	X(MATTRESS)				\




typedef enum core_part_id
{
	CORE_PART_INVALID,
	CORE_PART_REGFILE,
	CORE_PART_LSU,
	CORE_PART_CACHE,
	CORE_PART_AGGREGATOR,
	CORE_PART_FETCHER,
	CORE_PART_MATTRESS,
}core_part_id_t;


typedef struct
{
	part_t parts[10];

}core_t;

#endif