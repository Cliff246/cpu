#include "fetcher_interface.h"
#include "fetcher_entry.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

static void wcpu_fetcher_interface_print_imports(fetcher_interface_t *interface)
{
	for(int x = 0; x < WCPU_PART_FETCHER_INTERFACE_IMPORT_SIZE; ++x)
	{
		wcpu_part_fetcher_print_entry(interface->imports[x]);

	}
}

static void wcpu_fetcher_interface_print_ready(fetcher_interface_t *interface)
{
	for(int x = 0; x < WCPU_PART_FETCHER_INTERFACE_IMPORT_SIZE; ++x)
	{
		wcpu_part_fetcher_print_entry(interface->ready[x]);

	}
}

fetcher_interface_t *wcpu_fetcher_interface_create(void)
{
	fetcher_interface_t *interface = calloc(1, sizeof(fetcher_interface_t));
	assert(interface);
	//memset(interface->imports_used, 0, sizeof(bool) * WCPU_PART_FETCHER_INTERFACE_IMPORT_SIZE);
	return interface;
}



void wcpu_fetcher_interface_add_import(fetcher_interface_t *interface, uint64_t address)
{
	assert(interface);
	int index = -1;
	for(int x = 0; x < WCPU_PART_FETCHER_INTERFACE_IMPORT_SIZE; ++x)
	{
		if(interface->imports[x].ready == true && interface->imports[x].address == address)
		{
			return;
		}
		if(interface->imports[x].ready == false)
		{
			index = x;
		}

	}
	assert(index >= 0 && index < WCPU_PART_FETCHER_INTERFACE_IMPORT_SIZE);
	//printf("adding index:%d %lld\n", index, address);

	fetcher_entry_t entry = {.address = address, .data = 0, .ready =true, .time = 0};
	interface->imports[index] = entry;
}

void wcpu_fetcher_interface_mark_import(fetcher_interface_t *interface, uint64_t address, int64_t data)
{
	assert(interface);
	int index = -1;
	for(int x = 0; x < WCPU_PART_FETCHER_INTERFACE_IMPORT_SIZE; ++x)
	{
		if(interface->imports[x].ready == true && interface->imports[x].address == address)
		{
			index = x;
			interface->imports_used[x] = false;
		}


	}
	assert(index >= 0 && index < WCPU_PART_FETCHER_INTERFACE_IMPORT_SIZE);
	fetcher_entry_t entry = {.address = address, .data = data, .time = interface->imports[index].time, .ready = true};
	memset(&interface->imports[index], 0, sizeof(fetcher_entry_t));
	bool foundspot = false;
	for(int y = 0; y < WCPU_PART_FETCHER_INTERFACE_IMPORT_SIZE; ++y)
	{
		if(interface->ready[y].ready == false)
		{
			interface->ready[y] = entry;
			foundspot = true;
			break;
		}
	}
	assert(foundspot);
}

bool wcpu_fetcher_interface_use_import(fetcher_interface_t *interface, fetcher_entry_t *entry)
{

	//wcpu_fetcher_interface_print_imports(interface);
	int count = 0;
	bool found = false;
	for(int x = 0; x < WCPU_PART_FETCHER_INTERFACE_IMPORT_SIZE; ++x)
	{
		if(interface->imports[x].ready == true && interface->imports_used[x] == false)
		{
			//printf("used import %d\n", x);
			interface->imports_used[x] = true;
			*entry = interface->imports[x];
			found = true;
			return true;
		}
	}
	return found;
}

bool wcpu_fetcher_interface_read_ready(fetcher_interface_t *interface, fetcher_entry_t **entry)
{
	wcpu_fetcher_interface_print_ready(interface);

	assert(interface);
	int index = 0;
	for(int i = 0; i < WCPU_PART_FETCHER_INTERFACE_IMPORT_SIZE; ++i)
	{
		if(interface->ready[i].ready == true && i >= interface->ready_count)
		{
			*entry = &interface->ready[i];
			interface->ready_count++;
			return true;
		}
	}


	return false;
}

void wcpu_fetcher_interface_clean_ready(fetcher_interface_t *interface)
{
	assert(interface);
	for(int i = 0; i < WCPU_PART_FETCHER_INTERFACE_IMPORT_SIZE; ++i)
	{
		if(interface->ready[i].ready == true)
		{
			memset(&interface->ready[i], 0, sizeof(fetcher_entry_t));
		}
	}

}


bool wcpu_fetcher_interface_read_export(fetcher_interface_t *interface, part_signal_t **signal)
{
	assert(interface );

	bool found = false;
	int index = 0;
	for(int i = 0; i < WCPU_PART_FETCHER_INTERFACE_EXPORT_SIZE; ++i)
	{
		if(interface->exports[i] != NULL)
		{
			if(index >= interface->exports_count)
			{
				found = true;
				*signal = interface->exports[i];
				break;

			}
			index++;
		}
	}

	if(found)
	{
		interface->exports_count++;
	}
	return found;

}

bool wcpu_fetcher_interface_add_export(fetcher_interface_t *interface, part_signal_t *signal)
{
	assert(interface && signal);

	bool found = false;
	int index = -1;
	for(int i = 0; i < WCPU_PART_FETCHER_INTERFACE_EXPORT_SIZE; ++i)
	{
		if(interface->exports[i] == NULL)
		{
			index = i;
			found = true;
			break;
		}
	}

	if(found)
	{
		interface->exports[index] = signal;
	}
	return found;

}

bool wcpu_fetcher_interface_pop_export(fetcher_interface_t *interface, part_signal_t **signal)
{
	assert(interface );

	bool found = false;
	int index = -1;
	for(int i = 0; i < WCPU_PART_FETCHER_INTERFACE_EXPORT_SIZE; ++i)
	{
		if(interface->exports[i] != NULL)
		{
			found = true;
			index = i;
			break;
		}
	}

	if(found)
	{
		*signal = interface->exports[index];
		interface->exports[index] = NULL;
	}
	return found;
}

void wcpu_fetcher_interface_update(fetcher_interface_t *interface)
{
	assert(interface);


	for(int i = 0; i < WCPU_PART_FETCHER_INTERFACE_IMPORT_SIZE; ++i)
	{
		if(interface->imports[i].ready == true)
		{
			interface->imports[i].time += 1;
		}
	}

}

void wcpu_fetcher_interface_flush(fetcher_interface_t *interface)
{
	assert(interface);

}

void wcpu_fetcher_interface_clear_iterators(fetcher_interface_t *interface)
{
	interface->exports_count = 0;
	interface->ready_count = 0;


}


