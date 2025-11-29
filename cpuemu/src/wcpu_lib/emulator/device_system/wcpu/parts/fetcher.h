#ifndef __FETCHER_HEADER__
#define __FETCHER_HEADER__

#include "wcpu_part_ptr.h"
#include "wcpu_part.h"
#include <stdint.h>
#include <stdbool.h>
#include "wcpu_part_signal.h"

#define FETCHER_INSTRUCTION_BUFFER 16
#define FETCHER_IMMEDIATE_BUFFER 4
#define FETCHER_REQUEST_BUFFER 4


typedef struct fetch_request
{
	int64_t address;

}fetch_request_t;

typedef struct wcpu_part_fetcher
{
	int32_t insts[FETCHER_INSTRUCTION_BUFFER];
	int64_t imms[FETCHER_IMMEDIATE_BUFFER];
}fetcher_t;


part_ptr_t wcpu_fetcher_generate(void);

void wcpu_fetcher_update(part_t *fetcher);
void wcpu_fetcher_import( part_t *part, part_signal_t *signal);
bool wcpu_fetcher_export( part_t *part, part_signal_t **signal);

uint32_t get_inst_at_pc_address(uint64_t address);


#endif