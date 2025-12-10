#ifndef __WCPU_PART_FETCHER_CODE_DESCRIPTOR_PORT_ORDER_HEADER__
#define __WCPU_PART_FETCHER_CODE_DESCRIPTOR_PORT_ORDER_HEADER__

#include <stdint.h>

typedef struct wcpu_fetcher_port_order fetcher_port_order_t;


struct wcpu_fetcher_port_order_CD_SWAP
{
	uint64_t load_address;
};

fetcher_port_order_t *fetcher_port_code_description_order_create(uint64_t load_address);


#endif