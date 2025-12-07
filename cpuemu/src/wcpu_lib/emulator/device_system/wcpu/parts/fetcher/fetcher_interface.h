#ifndef __WCPU_PART_FETCHER_INTERFACE_HEADER__
#define __WCPU_PART_FETCHER_INTERFACE_HEADER__

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "wcpu_part_signal.h"
#include "fetcher_entry.h"

#define WCPU_PART_FETCHER_INTERFACE_IMPORT_SIZE 10
#define WCPU_PART_FETCHER_INTERFACE_EXPORT_SIZE 10

typedef struct wcpu_fetcher_interface
{
	int exports_count;
	part_signal_t *exports[WCPU_PART_FETCHER_INTERFACE_EXPORT_SIZE];


	bool imports_used[WCPU_PART_FETCHER_INTERFACE_IMPORT_SIZE];
	fetcher_entry_t imports[WCPU_PART_FETCHER_INTERFACE_IMPORT_SIZE];

	int ready_count;
	fetcher_entry_t ready[WCPU_PART_FETCHER_INTERFACE_IMPORT_SIZE];

}fetcher_interface_t;



fetcher_interface_t *wcpu_fetcher_interface_create(void);

//add a address to import
void wcpu_fetcher_interface_add_import(fetcher_interface_t *interface, uint64_t address);
//marks any import with the address
void wcpu_fetcher_interface_mark_import(fetcher_interface_t *interface, uint64_t address, int64_t data);
//keeps reading and gives false on no more imports
bool wcpu_fetcher_interface_use_import(fetcher_interface_t *interface, fetcher_entry_t *entry);

//reads the entry from the ready interface
bool wcpu_fetcher_interface_read_ready(fetcher_interface_t *interface, fetcher_entry_t **entry);
void wcpu_fetcher_interface_clean_ready(fetcher_interface_t *interface);

//exports the signals wanted
bool wcpu_fetcher_interface_read_export(fetcher_interface_t *interface, part_signal_t **signal);

//adds export signal
bool wcpu_fetcher_interface_add_export(fetcher_interface_t *interface, part_signal_t *signal);
bool wcpu_fetcher_interface_pop_export(fetcher_interface_t *interface, part_signal_t **signal);


void wcpu_fetcher_interface_update(fetcher_interface_t *interface);
void wcpu_fetcher_interface_flush(fetcher_interface_t *interface);
void wcpu_fetcher_interface_clear_iterators(fetcher_interface_t *interface);




#endif