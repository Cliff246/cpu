#ifndef __WCPU_PART_FETCHER_CONTROLLER_HEADER__
#define __WCPU_PART_FETCHER_CONTROLLER_HEADER__

#include <stdbool.h>
#include <stdint.h>


#include "fetcher_entry.h"
#include "fetcher_port.h"
#include "fetcher_interface.h"




typedef struct wcpu_fetcher_controller
{



	fetcher_port_order_ptr_t orders[WCPU_FETCHER_PORTS_COUNT];
	fetcher_port_t *ports[WCPU_FETCHER_PORTS_COUNT];
	uint32_t cap_mask;
}fetcher_controller_t;

fetcher_controller_t *wcpu_fetcher_controller_create(void);

void wcpu_fetcher_controller_update(fetcher_controller_t *controller, fetcher_interface_t *interface);



#endif