#ifndef __WS_SIM_VISTABLE_HEADER__
#define __WS_SIM_VISTABLE_HEADER__

#include "SIM_channel.h"
#include "SIM_chnlcfg.h"
#include "SIM_commons.h"
#include <stdint.h>

typedef struct WS_SIM_context SIM_context_t;



typedef struct WS_SIM_viselm
{
	SIM_devcfg_t *device;
	bool has_address;
	uint64_t address;
	uint64_t length;
	bool pretag;
	SIM_dtag_t tag;

}SIM_viselm_t;


void SIM_print_viselm(SIM_viselm_t *elm);
SIM_dtag_t SIM_init_viselm(SIM_viselm_t *elm, SIM_devcfg_t *device);


typedef struct WS_SIM_visaddr
{
	SIM_viselm_t *elm;
	uint64_t base;
	uint64_t size;
}SIM_visaddr_t;

bool SIM_init_visaddr(SIM_visaddr_t *address, SIM_viselm_t *elm);
static inline bool SIM_overlap_visaddr(SIM_visaddr_t *addr1, SIM_visaddr_t *addr2);
static void SIM_swap_visaddr_list(SIM_visaddr_t *list, uint64_t i, uint64_t j);
static uint64_t SIM_partition_visaddr_list(SIM_visaddr_t *list, uint64_t lo, uint64_t hi);
static void SIM_qsort_visaddr_list(SIM_visaddr_t *list, uint64_t lo, uint64_t hi);
static SIM_visaddr_t *SIM_bsearch_visaddr_list(SIM_visaddr_t *list, uint64_t size, uint64_t i);
static bool SIM_check_no_collisions_visaddr_list(SIM_visaddr_t *list, uint64_t size);

typedef struct WS_SIM_vistable
{
	uint64_t count;
	SIM_viselm_t *table;

	SIM_viselm_t **tagmap;


	uint64_t addrmap_count;
	SIM_visaddr_t *addrmap;

}SIM_vistable_t;


SIM_vistable_t *SIM_init_vistable(SIM_context_t *ctx);
static void SIM_init_vistable_tags(SIM_vistable_t *table, SIM_context_t *ctx);
static void SIM_init_vistable_tagmap(SIM_vistable_t *table, SIM_context_t *ctx);
static void SIM_init_vistable_addrmap(SIM_vistable_t *table, SIM_context_t *ctx);
static bool SIM_init_vistable_addrmap_align(SIM_vistable_t *table, SIM_context_t *ctx);

bool SIM_add_from_key_vistable(SIM_vistable_t *table, SIM_viselm_t *elm, SIM_dtag_t tag);
SIM_viselm_t *SIM_get_from_key_vistable(SIM_vistable_t *table, SIM_dtag_t tag);
SIM_viselm_t *SIM_get_from_addr_vistable(SIM_vistable_t *table, uint64_t addr);
SIM_viselm_t *SIM_get_from_index_vistable(SIM_vistable_t *table, uint64_t i);
void SIM_free_vistable(SIM_vistable_t *table);

void SIM_print_vistable_table(SIM_vistable_t *table);

void SIM_print_vistable_tagmap(SIM_vistable_t *table);
void SIM_print_vistable_addrmap(SIM_vistable_t *table);
#endif