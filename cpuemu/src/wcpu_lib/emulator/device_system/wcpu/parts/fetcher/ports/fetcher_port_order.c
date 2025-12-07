#include "fetcher_port_order.h"
#include <stdlib.h>
#include <assert.h>

fetcher_port_order_ptr_t fetcher_port_code_description_order_create(bool set_load_address, uint64_t load_address)
{
	fetcher_port_code_descriptor_order_t *cdo = calloc(1, sizeof(fetcher_port_code_descriptor_order_t));
	assert(cdo);
	cdo->set_load_address = set_load_address;
	cdo->load_address = load_address;


	fetcher_port_order_ptr_t order;
	order.cd = cdo;
	return order;

}