#ifndef __WCPU_PART_FETCHER_PORT_ORDER_HEADER__
#define __WCPU_PART_FETCHER_PORT_ORDER_HEADER__

#include <stdbool.h>
#include <stdint.h>



#define FETCHER_PORT_ORDER_LIST(X)\
	X(IMMEDIATE)				  \
	X(INSTRUCTION)				  \
	X(CODE_TABLE)				  \




#define FETCHER_PORT_ORDER_NAME(X) FETCHER_PORT_ORDER_TYPE_ ## X
#define FETCHER_PORT_ORDER_ENUM(X) FETCHER_PORT_ORDER_NAME(X),
typedef enum wcpu_fetcher_port_order_type
{
	FETCHER_PORT_ORDER_LIST(FETCHER_PORT_ORDER_ENUM)
}fetcher_port_order_type_t;

#define FETCHER_PORT_ORDER_STRUCT_NAME(X) wcpu_fetcher_port_order_ ## X

#define FETCHER_PORT_ORDER_STRUCT_TYPEDEF_NAME(X) fetcher_port_order_ ## X ## _t


#define FETCHER_PORT_ORDER_STRUCT(X) typedef struct FETCHER_PORT_ORDER_STRUCT_NAME(X) FETCHER_PORT_ORDER_STRUCT_TYPEDEF_NAME(X);
FETCHER_PORT_ORDER_LIST(FETCHER_PORT_ORDER_STRUCT)

typedef struct wcpu_fetcher_port_order fetcher_port_order_t;




struct wcpu_fetcher_port_order_IMMEDIATE
{
	uint64_t imm_base;
	uint64_t imm_len;
};

struct wcpu_fetcher_port_order_INSTRUCTION
{
	uint64_t ins_base;
	uint64_t ins_len;
};

struct wcpu_fetcher_port_order_CODE_TABLE
{

	uint64_t ct_base;
	uint64_t ct_len;

};

#define FETCHER_PORT_ORDER_UNION_ELEMENT(X) X
#define FETCHER_PORT_ORDER_UNION(X) FETCHER_PORT_ORDER_STRUCT_TYPEDEF_NAME(X)  FETCHER_PORT_ORDER_UNION_ELEMENT(X);


typedef union wcpu_fetcher_port_order_union
{
	FETCHER_PORT_ORDER_LIST(FETCHER_PORT_ORDER_UNION)

}fetcher_port_order_union_t;

typedef struct wcpu_fetcher_port_order
{
	fetcher_port_order_type_t type;
	fetcher_port_order_union_t order;
}fetcher_port_order_t;




#endif

