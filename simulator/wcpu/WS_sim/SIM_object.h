#ifndef __WS_SIM_OBJECT_HEADER__
#define __WS_SIM_OBJECT_HEADER__


#include "OBJ_bundle.h"
#include "OBJ_context.h"
#include "OBJ_handle.h"
#include "OBJ_locals.h"
#include "OBJ_pipeline.h"

#include "SIM_channel.h"
#include "SIM_commons.h"
#include "SIM_handle.h"

#include "OBJ_constants.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>



typedef struct WS_SIM_object
{

	SIM_port_global_t port_index;

	OBJ_context_t *context;
	OBJ_handle_t *obj_handle;
	SIM_handle_t *sim_handle;

	OBJ_locals_t *locals;


	//object pipeline for the
	OBJ_pipeline_t pipeline;
}SIM_object_t;

void SIM_object_read(SIM_object_t *object, OBJ_bundle_t *bnd);
void SIM_object_update(SIM_object_t *object);
void SIM_object_write(SIM_object_t *object, OBJ_bundle_t *bnd);

#endif