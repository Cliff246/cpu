#ifndef __WCPU_PART_MATTRESS_HEADER__
#define __WCPU_PART_MATTRESS_HEADER__


#include "fabric.h"
#include "mattress_packet.h"
#include "mattress_interface.h"
#include "mattress_unit.h"
#include "wcpu_part.h"

#define WCPU_MATTRESS_UNITS_COUNT 4


#define WCPU_MATTRESS_PACKETS_INFLIGHT 1000


typedef enum wcpu_mattress_debug_state
{
	WCPU_MATTRESS_DEBUG_NONE,
	WCPU_MATTRESS_DEBUG_LOW,
	WCPU_MATTRESS_DEBUG_HIGH,
	WCPU_MATTRESS_DEBUG_STEP,
}mattress_debug_flag_t;


typedef struct wcpu_part_mattress
{
	mattress_debug_flag_t flag;

	mattress_unit_t units[WCPU_MATTRESS_UNITS_COUNT];
	mattress_packet_t inflight[WCPU_MATTRESS_PACKETS_INFLIGHT];
}mattress_t;


part_ptr_t wcpu_mattress_generate(void);
void wcpu_mattress_update(part_t *part);

bool wcpu_mattress_import( part_t *part, part_signal_t *signal);
bool wcpu_mattress_export( part_t *part, part_signal_t **signal);

#endif