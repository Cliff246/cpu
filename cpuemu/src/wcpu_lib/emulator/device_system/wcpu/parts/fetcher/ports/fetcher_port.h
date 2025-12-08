#ifndef __WCPU_PART_FETCHER_PORT_HEADER__
#define __WCPU_PART_FETCHER_PORT_HEADER__

#include "fetcher_port_ptr.h"
#include "fetcher_port_order.h"


#include "fetcher_entry.h"
#include "fetcher_interface.h"
#include "wcpu_part_signal.h"

#include <stdint.h>
#include <stdbool.h>


typedef enum wcpu_fetcher_port_state_type
{
	FETCHER_PORT_INVAL,
	FETCHER_PORT_FLUSH,
	FETCHER_PORT_IDLE,
	FETCHER_PORT_ISSUE,
	FETCHER_PORT_WAITING,
	FETCHER_PORT_DONE,
	FETCHER_PORT_HOLD,
}fetcher_port_state_type_t;





typedef struct wcpu_fetcher_port
{
	fetcher_port_type_t type;
	fetcher_port_state_type_t state;

	//ready for next stage
	fetcher_port_ptr_t port;
	uint32_t requires_mask;
	uint32_t produces_mask;
	bool ready;

}fetcher_port_t;


typedef void (*wcpu_fetcher_port_create_fn)(fetcher_port_t *port);
//issues the next fetchers state
typedef uint64_t (*wcpu_fetcher_port_issue_fn)(fetcher_port_t *port);
//polls the fetcher returns true on poll recongnized, returns false on non
typedef bool (*wcpu_fetcher_port_poll_fn)(fetcher_port_t *port, uint64_t address);
//this imports the data into the port,
typedef void (*wcpu_fetcher_port_import_fn)(fetcher_port_t *port, uint64_t address, int64_t data);
//exports the fetcher content
typedef part_signal_t *(*wcpu_fetcher_port_export_fn)(fetcher_port_t *port);
//flushes the fetchers data
typedef void (*wcpu_fetcher_port_flush_fn)(fetcher_port_t *port);


//cycle, this checks if the port goes to export or backs to issue
typedef bool (*wcpu_fetcher_port_cycle_fn)(fetcher_port_t *port);

//order in the fetcher
typedef void (*wcpu_fetcher_port_order_fn)(fetcher_port_t *port, fetcher_port_order_ptr_t order);
typedef void (*wcpu_fetcher_port_order_free_fn)(fetcher_port_order_ptr_t order);


typedef struct wcpu_fetcher_port_class
{
	wcpu_fetcher_port_create_fn create;
	wcpu_fetcher_port_issue_fn issue;
	wcpu_fetcher_port_poll_fn poll;
	wcpu_fetcher_port_import_fn import;
	wcpu_fetcher_port_cycle_fn cycle;
	wcpu_fetcher_port_export_fn export;
	wcpu_fetcher_port_flush_fn flush;


	wcpu_fetcher_port_order_fn order;
	wcpu_fetcher_port_order_free_fn free_order;
}fetcher_port_class_t;

extern fetcher_port_class_t fetcher_port_vtable[WCPU_FETCHER_PORTS_COUNT];


fetcher_port_t *wcpu_fetcher_port_create(fetcher_port_type_t type);
uint32_t wcpu_fetcher_port_advance(fetcher_port_t *port, uint32_t cap_mask, fetcher_interface_t *interface);
//free's the order after, DONT USE AFTER
void wcpu_fetcher_port_consume_order(fetcher_port_t *port, fetcher_port_order_ptr_t order);

void wcpu_fetcher_port_set_state(fetcher_port_t *port, fetcher_port_state_type_t state);

#endif