#ifndef __WS_OBJ_PIPELINE_HEADER__
#define __WS_OBJ_PIPELINE_HEADER__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "OBJ_node.h"

typedef struct WS_OBJ_stage
{
	size_t size;
	OBJ_node_t *nodes;

}OBJ_stage_t;

typedef struct WS_OBJ_pipeline
{
	
	size_t size;
	OBJ_stage_t *stages;
}OBJ_pipeline_t;

#endif