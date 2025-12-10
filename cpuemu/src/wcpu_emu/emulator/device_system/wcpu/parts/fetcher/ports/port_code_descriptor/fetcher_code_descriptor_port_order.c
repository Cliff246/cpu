#include "fetcher_code_descriptor_port_order.h"

#include "fetcher_port_order.h"
#include <stdlib.h>
#include <assert.h>

fetcher_port_order_t *fetcher_port_code_description_order_create(uint64_t load_address)
{


	fetcher_port_order_t *ptr = calloc(1, sizeof(fetcher_port_order_t));
	assert(ptr);
	ptr->type = FETCHER_PORT_ORDER_TYPE_CD_SWAP;
	ptr->order.CD_SWAP.load_address = load_address;




	return ptr;
}