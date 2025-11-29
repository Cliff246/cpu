#ifndef __WCPU_PART_PTR_HEADER__
#define __WCPU_PART_PTR_HEADER__

typedef struct wcpu_part_fetcher 		fetcher_t;
typedef struct wcpu_part_mau 			mau_t;
typedef struct wcpu_part_regfile 		regfile_t;
typedef struct wcpu_part_mattress 		mattress_t;
typedef struct wcpu_part_ledger 		ledger_t;
typedef struct wcpu_part_cache 			cache_t;
typedef struct wcpu_part_lsu 			lsu_t;
typedef struct wcpu_part_aggregator 	aggregator_t;


typedef union wcpu_part_ptr
{
	mau_t *mau;
	fetcher_t *fetcher;
	regfile_t *regfile;
	mattress_t *mattress;
	ledger_t *ledger;
	cache_t *cache;
	lsu_t *lsu;
	aggregator_t *aggregator;
	void *raw;
}part_ptr_t;

typedef struct wcpu_part part_t;


#endif