#ifndef __WCPU_PART_HEADER__
#define __WCPU_PART_HEADER__

#include "fetcher.h"
#include "lsu.h"
#include "register_file.h"
#include "mattress.h"
#include "aggregator.h"
#include "cache.h"
#include "ledger.h"
#include "mattress.h"
#include "fabric.h"

#include "wcpu_isa.h"


typedef enum wcpu_part_type
{
	PART_INVAL,
	PART_LSU,
	PART_MATTRESS,
	PART_REGISTER_FILE,
	PART_FETCHER,
	PART_CACHE,
	PART_AGGREGATOR,
	PART_LEDGER,
	PART_MATTRESS,

}part_type_t;

typedef union wcpu_part_impl
{
	lsu_t *lsu;
	fetcher_t *fetcher;
	aggregator_t *aggregator;
	regfile_t *regfile;
	cache_t *cache;
	ledger_t *ledger;
	fabric_t *fabric;
}part_impl_t;

typedef struct wcpu_part
{
	part_type_t type;
	part_impl_t impl;
}part_t;




#endif