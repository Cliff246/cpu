#ifndef __WS_SIM_OBJECT_HEADER__
#define __WS_SIM_OBJECT_HEADER__


#include "OBJ_context.h"
#include "OBJ_handle.h"
#include "OBJ_locals.h"
#include "OBJ_pipeline.h"

#include "SIM_channel.h"
#include "SIM_handle.h"

#include "OBJ_constants.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>



typedef struct WS_SIM_object
{

	uint64_t port_index;

	OBJ_ctx_t *ctx;
	OBJ_hnd_t *obj_hnd;
	SIM_handle_t *sim_handle;

	OBJ_locals_t *locals;


	//object pipeline for the
	OBJ_pipeline_t pipeline;
}SIM_object_t;



#endif