#ifndef __MATTRESS_HEADER__
#define __MATTRESS_HEADER__

#define MATTRESS_TOTAL_FABRICS 4

#include "fabric.h"
#include "lsu.h"
#include "register_file.h"
#include "ledger.h"
#include "mattress_packet.h"
#include "aggregator.h"
#include "mattress_interface.h"
#include "wcpu_part.h"

typedef enum mattress_unit_type
{
	MATTRESS_UNIT_INVALID,
	MATTRESS_UNIT_FABRIC,
	MATTRESS_UNIT_LSU,
	MATTRESS_UNIT_LEDGER,
	MATTRESS_UNIT_REGFILE,
	MATTRESS_UNIT_AGGREGATOR,
	MATTRESS_UNIT_ALU
}mattress_unit_type_t;

typedef struct mattress_unit
{
	mattress_unit_type_t type;

	union
	{
		fabric_t *fabric;
		lsu_t *lsu;
		regfile_t *regfile;
		ledger_t *ledger;
		aggregator_t *aggregator;
	}unit;

	mat_port_t *port;

}mattress_unit_t;



//supposed to be 'component' connected to the mattress
mattress_unit_t *wcpu_mattress_unit_generate(mattress_unit_type_t type);
void wcpu_mattress_unit_update(mattress_unit_t *unit);



typedef struct mattress
{

	mattress_unit_t units[MATTRESS_TOTAL_FABRICS];

}mattress_t;


part_ptr_t wcpu_mattress_generate(void);
void wcpu_mattress_update(part_t *mattress);

#endif