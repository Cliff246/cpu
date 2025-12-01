#include "lsu.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "wcpu_part_ptr.h"
#include "wcpu_part_signal_lsu_entry.h"
#include "wcpu_part_signal_core_io.h"


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
//
void wcpu_lsu_entry_set_value(part_t *part, int index, int64_t value)
{
	assert(part != NULL && "part cannot be null");
	assert(part->type == WCPU_PART_LSU && "part type for import must be of type WCPU_PART_LSU");
	assert(index >= 0 && index < MAX_LSU_ENTRIES && "index must be in valid range");

	lsu_t *lsu = part->ptr.lsu;
	lsu->entries[index].value = value;

}

void wcpu_lsu_entry_set_finished(part_t *part, int index, bool value)
{
	assert(part != NULL && "part cannot be null");
	assert(part->type == WCPU_PART_LSU && "part type for import must be of type WCPU_PART_LSU");
	assert(index >= 0 && index < MAX_LSU_ENTRIES && "index must be in valid range");

	lsu_t *lsu = part->ptr.lsu;
	lsu->entries[index].finished = value;

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
	lsu_t *lsu = part->ptr.lsu;

	//this sets the part to find
	if(signal->signal_type == PART_SIGNAL_TYPE_CORE_MEM_RESPONSE)
	{


		//linear search for entry with the signals

		_part_signal_CORE_MEM_RESPONSE_t mem_response = *signal->ptr.CORE_MEM_RESPONSE;
		//checkk and ready the entry that finished read
		bool success = false;
		for(int i = 0; i < MAX_LSU_ENTRIES; ++i)
		{
			lsu_entry_t entry = wcpu_lsu_get(part, i);
			if(entry.awaiting == true && entry.type == LSU_ENTRY_READ && entry.address == mem_response.address)
			{
				wcpu_lsu_entry_set_finished(part, i, true);
				wcpu_lsu_entry_set_value(part, i, mem_response.load_value);
				success = true;
			}
		}
		assert(success == true && "must find entry in that matches load");
		part_signal_consume(signal);
	}
	else
	{
		assert(signal->signal_type == PART_SIGNAL_TYPE_LSU && "signal must be of type lsu");

		assert(lsu->entries_currently < MAX_LSU_ENTRIES && "lsu entries currently cannot be greater than the max inflight lsu entries");
		_part_signal_LSU_t *signal_content = signal->ptr.LSU;
		lsu_entry_type_t type = (signal_content->loadstore == true)? LSU_ENTRY_READ : LSU_ENTRY_WIRTE;

		wcpu_lsu_entry_set(part, lsu->entries_currently, signal_content->address, signal_content->value, type, signal->src_id);
		part_signal_consume(signal);
		lsu->entries_currently++;
	}

}


//export to the lsu entry thats avilable
bool wcpu_lsu_export( part_t *part, part_signal_t **signal)
{
	assert(part != NULL && "part cannot be null");
	assert(part->type == WCPU_PART_LSU && "part type for import must be of type WCPU_PART_LSU");
	lsu_t *lsu = part->ptr.lsu;


	if(lsu->input_signal.release == true)
	{

		_part_signal_CORE_INPUT_t *corein = calloc(1, sizeof(_part_signal_CORE_INPUT_t));
		assert(corein != NULL && "cannot fail calloc");


		corein->addr = lsu->input_signal.addr;

		part_signal_content_ptr_t pscp;
		pscp.CORE_INPUT = corein;

		part_signal_t *psig = part_signal_create(PART_SIGNAL_TYPE_CORE_INPUT, part->id, -1, pscp);
		*signal = psig;


		lsu->input_signal.release = false;
		return true;
	}
	if(lsu->output_signal.release == true)
	{

		_part_signal_CORE_OUTPUT_t *coreout = calloc(1, sizeof(_part_signal_CORE_OUTPUT_t));
		assert(coreout != NULL && "cannot fail calloc");


		coreout->addr = lsu->output_signal.addr;
		coreout->value = lsu->output_signal.value;

		part_signal_content_ptr_t pscp;
		pscp.CORE_OUTPUT = coreout;

		part_signal_t *psig =  part_signal_create(PART_SIGNAL_TYPE_CORE_OUTPUT, part->id, -1, pscp);
		*signal = psig;
		lsu->output_signal.release = false;

		return true;
	}

	if(lsu->entries_currently > 0)
	{
		lsu_entry_t entry =  wcpu_lsu_get(part, lsu->entries_currently - 1);

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


