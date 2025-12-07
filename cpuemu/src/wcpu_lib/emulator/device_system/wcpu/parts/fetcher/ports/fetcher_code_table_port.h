#ifndef __WCPU_PART_FETCHER_CODE_TABLE_PORT_HEADER__
#define __WCPU_PART_FETCHER_CODE_TABLE_PORT_HEADER__


#include "fetcher_port_ptr.h"
#include "fetcher_port.h"
#include "wcpu_part_signal.h"
#include <stdint.h>
#include <stdbool.h>


struct wcpu_fetcher_port_code_table
{
	uint64_t buffer[64];
	uint64_t ct_start;
	uint64_t ct_length;
	uint32_t index;

};




void fetcher_port_code_table_create(fetcher_port_t *port);
uint64_t fetcher_port_code_table_issue(fetcher_port_t *port);
bool fetcher_port_code_table_poll(fetcher_port_t *port, uint64_t address);
void fetcher_port_code_table_import(fetcher_port_t *port, uint64_t address, int64_t data);
part_signal_t *fetcher_port_code_table_export(fetcher_port_t *port);
void fetcher_port_code_table_flush(fetcher_port_t *port);
bool fetcher_port_code_table_cycle(fetcher_port_t *port);

void fetcher_port_code_table_order(fetcher_port_t *port, fetcher_port_order_ptr_t order);
void fetcher_port_code_table_order_free(fetcher_port_order_ptr_t order);



#endif