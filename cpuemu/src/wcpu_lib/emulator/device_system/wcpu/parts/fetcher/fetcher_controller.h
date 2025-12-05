#ifndef __WCPU_PART_FETCHER_CONTROLLER_HEADER__
#define __WCPU_PART_FETCHER_CONTROLLER_HEADER__

#include <stdbool.h>
#include <stdint.h>


#include "fetcher_entry.h"
#include "fetcher_port.h"
#include "fetcher_interface.h"

typedef enum wcpu_fetcher_controller_state
{
	FETCHER_CONTROLLER_STATE_INVAL,
	FETCHER_CONTROLLER_STATE_LOAD_CODE,
	FETCHER_CONTROLLER_STATE_LOAD_TABLE,
	FETCHER_CONTROLLER_STATE_LOAD_DESC,
	FETCHER_CONTROLLER_STATE_FLUSH,
}fetcher_controller_state_t;





typedef struct wcpu_fetcher_controller
{
	fetcher_controller_state_t state;
	fetcher_port_t *ports[WCPU_FETCHER_PORTS_COUNT];

}fetcher_controller_t;

fetcher_controller_t *wcpu_fetcher_controller_create(void);

void wcpu_fetcher_controller_update(fetcher_controller_t *controller, fetcher_interface_t *interface);



#endif