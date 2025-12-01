#ifndef __LSU_HEADER__
#define __LSU_HEADER__
#include "wcpu_part_signal.h"

#include "wcpu_part.h"
#include <stdint.h>
#include <stdbool.h>

#define MAX_LSU_ENTRIES 128

typedef enum lsu_entry_type
{
	LSU_ENTRY_INVAL,
	LSU_ENTRY_READ,
	LSU_ENTRY_WIRTE,

}lsu_entry_type_t;

typedef struct lsu_entry
{

	int64_t address;
	int64_t value;
	lsu_entry_type_t type;

	int writeback_dst;
	bool awaiting;
	bool finished;
}lsu_entry_t;


typedef struct wcpu_part_lsu
{
	lsu_entry_t entries[MAX_LSU_ENTRIES];
	int entries_currently;

	struct {
		bool release;
		int64_t value;
		int64_t addr;

	}output_signal;

	struct {
		bool release;
		int64_t addr;
	}input_signal;

}lsu_t;

part_ptr_t wcpu_lsu_generate(void);
void wcpu_lsu_update(part_t *lsu);

void wcpu_lsu_import( part_t *part, part_signal_t *signal);
bool wcpu_lsu_export( part_t *part, part_signal_t **signal);


void wcpu_lsu_entry_clear(part_t *part, int index);

//part and index are where you want to set
//address, value, type, writeback_dest are the arguments
void wcpu_lsu_entry_set(part_t *part, int index, int64_t address, int64_t value, lsu_entry_type_t type, int writeback_dest);

lsu_entry_t wcpu_lsu_get(part_t *part, int index);
void wcpu_lsu_entry_set_value(part_t *part, int index, int64_t value);
void wcpu_lsu_entry_set_finished(part_t *part, int index, bool value);

uint64_t load(uint64_t address);

void store(uint64_t address, int64_t value);

uint64_t address(uint64_t addr);


#endif