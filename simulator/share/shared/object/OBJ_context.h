#ifndef __OBJ_CONTEXT_HEADER__
#define __OBJ_CONTEXT_HEADER__

#include <stdint.h>
#include <stdbool.h>
#include <stdalign.h>
#include "OBJ_handle.h"
#include "OBJ_resource.h"



typedef struct OBJ_silo
{
	OBJ_resrc_type_t type;
	uint64_t size;
	OBJ_resource_t *resrc;
}OBJ_silo_t;


//local context of data traveling between parts
typedef struct OBJ_context
{
	uint64_t inputs_size;
	uint64_t outputs_size;
	OBJ_silo_t *inputs;
	OBJ_silo_t *outputs;
}OBJ_context_t;

#endif