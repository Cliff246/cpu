#ifndef __WCPU_PART_FETCHER_IMMEDIATE_PORT_HEADER__
#define __WCPU_PART_FETCHER_IMMEDIATE_PORT_HEADER__

#include "fetcher_port_ptr.h"
#include "fetcher_port.h"
#include "wcpu_part_signal.h"

#include <stdint.h>
#include <stdbool.h>

struct wcpu_fetcher_port_immediate
{
	int64_t buffer[8];
	uint64_t ipc_start;
	uint64_t ipc_length;
	uint64_t base;
	uint32_t index;

};



void fetcher_port_immediate_create(fetcher_port_t *port);
uint64_t fetcher_port_immediate_issue(fetcher_port_t *port);
bool fetcher_port_immediate_poll(fetcher_port_t *port, uint64_t address);
void fetcher_port_immediate_import(fetcher_port_t *port, uint64_t address, int64_t data);
part_signal_t *fetcher_port_immediate_export(fetcher_port_t *port);
void fetcher_port_immediate_flush(fetcher_port_t *port);
bool fetcher_port_immediate_cycle(fetcher_port_t *port);

void fetcher_port_immediate_order(fetcher_port_t *port, fetcher_port_order_t *order);


#endif