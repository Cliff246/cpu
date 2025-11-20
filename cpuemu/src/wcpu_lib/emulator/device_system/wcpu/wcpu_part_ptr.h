#ifndef __WCPU_PART_PTR_HEADER__
#define __WCPU_PART_PTR_HEADER__

typedef struct fetcher fetcher_t;
typedef struct alu alu_t;
typedef struct regfile regfile_t;
typedef struct mattress mattress_t;
typedef struct ledger ledger_t;
typedef struct cache cache_t;
typedef struct lsu lsu_t;
typedef struct aggregator aggregator_t;


typedef union wcpu_part_ptr
{
	alu_t *alu;
	fetcher_t *fetcher;
	regfile_t *regfile;
	mattress_t *mattress;
	ledger_t *ledger;
	cache_t *cache;
	lsu_t *lsu;
	aggregator_t *aggregator;
	void *raw;
}part_ptr_t;
#endif