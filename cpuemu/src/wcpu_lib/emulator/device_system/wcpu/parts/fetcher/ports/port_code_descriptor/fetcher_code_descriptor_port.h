#ifndef __WCPU_PART_FETCHER_CODE_DESCRIPTOR_PORT_HEADER__
#define __WCPU_PART_FETCHER_CODE_DESCRIPTOR_PORT_HEADER__


#include "fetcher_port_ptr.h"
#include "fetcher_port.h"
#include "fetcher_code_descriptor_port_context.h"
#include "wcpu_part_signal.h"
#include "wcpu_isa.h"
#include <stdint.h>
#include <stdbool.h>


struct wcpu_fetcher_port_code_descriptor
{

	fetcher_port_cd_ctx_t active_context;

	wcpu_isa_code_description_t current_cd;
};

//create port
void fetcher_port_code_description_create(fetcher_port_t *port);

//issues the next address.
uint64_t fetcher_port_code_description_issue(fetcher_port_t *port);


//polls if the port is waiting on this address
bool fetcher_port_code_description_poll(fetcher_port_t *port, uint64_t address);


//imports the content and is essentially on a successful poll
void fetcher_port_code_description_import(fetcher_port_t *port, uint64_t address, int64_t data);

//final stage of port, exports new signal to dispatch to core
part_signal_t *fetcher_port_code_description_export(fetcher_port_t *port);


//flushes content of the port
void fetcher_port_code_description_flush(fetcher_port_t *port);


//cycles the fetcher and determines if export or new issue
bool fetcher_port_code_description_cycle(fetcher_port_t *port);

//orders
void fetcher_port_code_description_order(fetcher_port_t *port, fetcher_port_order_t *order);

//should be temp

#endif