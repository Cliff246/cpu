#ifndef __WCPU_PART_FETCHER_HEADER__
#define __WCPU_PART_FETCHER_HEADER__

#include "wcpu_part_ptr.h"
#include <stdint.h>
#include <stdbool.h>
#include "wcpu_part_signal.h"


#include "fetcher_code_descriptor.h"
#include "fetcher_controller.h"
#include "fetcher_buffer.h"

#define FETCHER_BUFFER_LIMIT 10


typedef struct wcpu_part_fetcher
{
	bool doload;
	int64_t toload;

	//temp bullshit should goto register file
	bool load_code_descriptor;
	bool finished_code_descriptor;
	uint64_t code_descriptor_address;
	int code_descriptor_count;
	int64_t code_descriptor_chunk[6];



	//newer ow

	fetcher_code_descriptor_t code_desc;
	fetcher_controller_t controller;
	fetcher_buffer_t buffers[FETCHER_BUFFER_LIMIT];
}fetcher_t;


part_ptr_t wcpu_fetcher_generate(void);

void wcpu_fetcher_update(part_t *part);
void wcpu_fetcher_import( part_t *part, part_signal_t *signal);
bool wcpu_fetcher_export( part_t *part, part_signal_t **signal);

uint32_t get_inst_at_pc_address(uint64_t address);


#endif