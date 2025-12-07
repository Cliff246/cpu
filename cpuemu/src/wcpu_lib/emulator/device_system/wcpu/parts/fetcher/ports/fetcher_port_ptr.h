#ifndef __WCPU_PART_FETCHER_PORT_PTR_HEADER__
#define __WCPU_PART_FETCHER_PORT_PTR_HEADER__

#define WCPU_FETCHER_PORTS_COUNT 4

typedef struct wcpu_fetcher_port_code_descriptor fetcher_port_code_descriptor_t;
typedef struct wcpu_fetcher_port_instruction fetcher_port_instruction_t;
typedef struct wcpu_fetcher_port_immediate fetcher_port_immediate_t;
typedef struct wcpu_fetcher_port_code_table fetcher_port_code_table_t;

typedef enum wcpu_fetcher_port_type
{
	WCPU_FETCHER_PORT_CODE_DESCRIPTOR,
	WCPU_FETCHER_PORT_INSTRUCTION,
	WCPU_FETCHER_PORT_IMMEDIATE,
	WCPU_FETCHER_PORT_CODE_TABLE,
}fetcher_port_type_t;

typedef union
{
	fetcher_port_code_descriptor_t *cd;
	fetcher_port_instruction_t *ins;
	fetcher_port_immediate_t *imm;
	fetcher_port_code_table_t *ct;
	void *raw;
}fetcher_port_ptr_t;

typedef enum wcpu_fetcher_port_capacity_flag
{
	FETCHER_PORT_CAPACITY_CODE_DESCRIPTOR = 1 << 0,
	FETCHER_PORT_CAPACITY_CODE_TABLE      = 1 << 1,
	FETCHER_PORT_CAPACITY_IMMEDIATE       = 1 << 2,
	FETCHER_PORT_CAPACITY_INSTRUCTION     = 1 << 3,
}fetcher_port_capactity_flag_t;



#endif