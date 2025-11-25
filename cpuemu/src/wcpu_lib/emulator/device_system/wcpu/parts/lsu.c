#include "lsu.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "wcpu_part_signal_lsu_entry.h"


void wcpu_lsu_entry_clear(part_t *part, int index)
{
	assert(part != NULL && "part cannot be null");
	assert(part->type == WCPU_PART_LSU && "part type for import must be of type WCPU_PART_LSU");
	assert(index >= 0 && index < MAX_LSU_ENTRIES && "index must be in valid range");

	lsu_t *lsu = part->ptr.lsu;

	memset(&lsu->entries[index], 0, sizeof(lsu_entry_t));
	return;
}
//part and index are where you want to set
//address, value, type, writeback_dest are the arguments
void wcpu_lsu_entry_set(part_t *part, int index, int64_t address, int64_t value, lsu_entry_type_t type, int writeback_dest)
{
	assert(part != NULL && "part cannot be null");
	assert(part->type == WCPU_PART_LSU && "part type for import must be of type WCPU_PART_LSU");
	assert(index >= 0 && index < MAX_LSU_ENTRIES && "index must be in valid range");

	lsu_t *lsu = part->ptr.lsu;
	assert(type != LSU_ENTRY_INVAL && "type cannot be invalid");



	lsu_entry_t entry = {.address = address, .writeback_dst = writeback_dest, .type = type, .value = value, .awaiting = false};
	if(type == LSU_ENTRY_READ)
	{
		entry.awaiting = true;
	}
	lsu->entries[index] = entry;
	return;
}

lsu_entry_t wcpu_lsu_get(part_t *part, int index)
{
	assert(part != NULL && "part cannot be null");
	assert(part->type == WCPU_PART_LSU && "part type for import must be of type WCPU_PART_LSU");
	assert(index >= 0 && index < MAX_LSU_ENTRIES && "index must be in valid range");

	lsu_t *lsu = part->ptr.lsu;

	return lsu->entries[index];
}

part_ptr_t wcpu_lsu_generate(void)
{
	lsu_t *lsu = calloc(1, sizeof(lsu_t));
	part_ptr_t pptr;
	pptr.lsu = lsu;
	return pptr;
}

void wcpu_lsu_update(part_t *lsu)
{
	//must fill in the device message loads and stores. then convert and correct the right entries for export. or skip the ones still loading
	//
}


void wcpu_lsu_import( part_t *part, part_signal_t *signal)
{
	assert(part != NULL && "part cannot be null");
	assert(part->type == WCPU_PART_LSU && "part type for import must be of type WCPU_PART_LSU");
	assert(signal->signal_type == PART_SIGNAL_TYPE_LSU && "signal must be of type lsu");
	lsu_t *lsu = part->ptr.lsu;

	assert(lsu->entries_currently < MAX_LSU_ENTRIES && "lsu entries currently cannot be greater than the max inflight lsu entries");
	_part_signal_LSU_t *signal_content = part_signal_consume(signal)->ptr.LSU;
	lsu_entry_type_t type = (signal_content->loadstore == true)? LSU_ENTRY_READ : LSU_ENTRY_WIRTE;

	wcpu_lsu_entry_set(part, lsu->entries_currently, signal_content->address, signal_content->value, type, signal->src_id);
	lsu->entries_currently++;


}


//export to the lsu entry thats avilable
bool wcpu_lsu_export( part_t *part, part_signal_t **signal)
{
	assert(part != NULL && "part cannot be null");
	assert(part->type == WCPU_PART_LSU && "part type for import must be of type WCPU_PART_LSU");
	lsu_t *lsu = part->ptr.lsu;

	if(lsu->entries_currently > 0)
	{
		lsu_entry_t entry =  wcpu_lsu_get(part, lsu->entries_currently);

		if(entry.awaiting == true && entry.finished == true)
		{
			//pull
		}
		else
		{
			//we need to keep pulling entries till we find one that can be delivered or we run out of options
		}

	}
	else
	{
		return false;
	}


	return false;
}


