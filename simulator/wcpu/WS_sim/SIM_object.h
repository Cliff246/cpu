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

	uint16_t chnl_start;
	uint16_t chnl_length;
	uint8_t chnl_ready[OBJ_MAX_CHANNELS];
	uint8_t chnl_ready_length;
	uint16_t routetable_id;
	OBJ_ctx_t *ctx;
	OBJ_hnd_t *obj_hnd;
	SIM_handle_t *sim_handle;

	OBJ_locals_t *locals;


	//object pipeline for the
	OBJ_pipeline_t pipeline;
	OBJ_bnd_t *bnd;
}SIM_object_t;


void SIM_object_append_bundle(SIM_object_t *obj, OBJ_msg_t msg);
OBJ_bnd_t *SIM_object_release_bundle(SIM_object_t *obj);

#endif