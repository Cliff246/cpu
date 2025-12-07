#include "fetcher_controller.h"
#include "fetcher_port.h"
#include <stdlib.h>

fetcher_controller_t *wcpu_fetcher_controller_create(void)
{
	fetcher_controller_t *controller = calloc(1, sizeof(fetcher_controller_t));



	controller->ports[WCPU_FETCHER_PORT_CODE_DESCRIPTOR] = wcpu_fetcher_port_create(WCPU_FETCHER_PORT_CODE_DESCRIPTOR);
	controller->ports[WCPU_FETCHER_PORT_INSTRUCTION] = wcpu_fetcher_port_create(WCPU_FETCHER_PORT_INSTRUCTION);
	controller->ports[WCPU_FETCHER_PORT_IMMEDIATE] = wcpu_fetcher_port_create(WCPU_FETCHER_PORT_IMMEDIATE);
	controller->ports[WCPU_FETCHER_PORT_CODE_TABLE] = wcpu_fetcher_port_create(WCPU_FETCHER_PORT_CODE_TABLE);

	return controller;
}




void wcpu_fetcher_controller_update(fetcher_controller_t *controller, fetcher_interface_t *interface)
{


	uint32_t masks;
	for(int i = 0; i < WCPU_FETCHER_PORTS_COUNT; ++i)
	{
		//clear the iterators for this interface set
		wcpu_fetcher_interface_clear_iterators(interface);
		masks |= wcpu_fetcher_port_advance(controller->ports[i], controller->cap_mask, interface);

	}
	controller->cap_mask = masks;


	wcpu_fetcher_interface_clear_iterators(interface);


}