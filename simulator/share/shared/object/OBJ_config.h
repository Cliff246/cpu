#ifndef __WS_OBJ_CONFIG_HEADER__
#define __WS_OBJ_CONFIG_HEADER__

#include <stdbool.h>
#include <stdint.h>

#include "OBJ_blueprint.h"
#include "OBJ_pipeline.h"

typedef struct WS_OBJ_config OBJ_cfg_t;


typedef bool (*OBJ_cfg_init_settings_t)(OBJ_cfg_t *cfg, )
typedef bool (*OBJ_cfg_init_pipeline_t)(OBJ_blueprint_t *bp, OBJ_pipeline_t *pl);

typedef struct WS_OBJ_config_vtable
{

}OBJ_cfg_vt_t;

struct WS_OBJ_config
{
	OBJ_cfg_vt_t *vt;
};

#endif