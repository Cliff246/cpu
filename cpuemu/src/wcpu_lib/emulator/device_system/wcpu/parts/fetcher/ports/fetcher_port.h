#ifndef __WCPU_PART_FETCHER_PORT_HEADER__
#define __WCPU_PART_FETCHER_PORT_HEADER__

#include "fetcher_port_ptr.h"
#include <stdint.h>

typedef enum wcpu_fetcher_port_state_type
{
	FETCHER_PORT_IDLE,
	FETCHER_PORT_ISSUE,
	FETCHER_PORT_WAITING,
	FETCHER_PORT_DONE,
}fetcher_port_state_type_t;



typedef struct wcpu_fetcher_port
{
	fetcher_port_type_t type;
	fetcher_port_state_type_t state;
	fetcher_port_ptr_t port;
	uint64_t address;
	int64_t value;
	bool valid;
	bool ready;
}fetcher_port_t;




#endif