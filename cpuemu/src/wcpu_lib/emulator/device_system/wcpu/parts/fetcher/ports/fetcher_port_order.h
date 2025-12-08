#ifndef __WCPU_PART_FETCHER_PORT_ORDER_HEADER__
#define __WCPU_PART_FETCHER_PORT_ORDER_HEADER__

#include <stdbool.h>
#include <stdint.h>

//orders
typedef struct wcpu_fetcher_port_code_descriptor_order fetcher_port_code_descriptor_order_t;
typedef struct wcpu_fetcher_port_instruction_order fetcher_port_instruction_order_t;
typedef struct wcpu_fetcher_port_immediate_order fetcher_port_immediate_order_t;
typedef struct wcpu_fetcher_port_code_table_order fetcher_port_code_table_order_t;


struct wcpu_fetcher_port_instruction_order
{

};

struct wcpu_fetcher_port_immediate_order
{

};

struct wcpu_fetcher_port_code_table_order
{
	uint64_t ct_base;
	uint64_t ct_len;
};

struct wcpu_fetcher_port_code_descriptor_order
{
	bool set_load_address;
	uint64_t load_address;
};

typedef union
{
	fetcher_port_code_descriptor_order_t *cd;
	fetcher_port_code_table_order_t *ct;
	fetcher_port_immediate_order_t *imm;
	fetcher_port_instruction_order_t *ins;
	void *raw;
}fetcher_port_order_ptr_t;

fetcher_port_order_ptr_t fetcher_port_code_description_order_create(bool set_load_address, uint64_t load_address);


#endif

