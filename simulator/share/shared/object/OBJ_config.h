#ifndef __WS_OBJ_CONFIG_HEADER__
#define __WS_OBJ_CONFIG_HEADER__

#include <stdbool.h>
#include <stdint.h>

#include "OBJ_blueprint.h"
#include "OBJ_pipeline.h"
#include "OBJ_setting.h"
typedef struct WS_OBJ_config OBJ_cfg_t;

typedef OBJ_cfg_t *(*OBJ_cfg_init_fn_t)(void);

//set the base settings
typedef bool (*OBJ_cfg_add_setting_fn_t)(OBJ_cfg_t *cfg, OBJ_setting_t *setting);
//set up the object's layout and bandwidth design
typedef bool (*OBJ_cfg_init_blueprint_fn_t)(OBJ_cfg_t *cfg, OBJ_blueprint_t *bp);
//set up how the design runs in the pipeline
typedef bool (*OBJ_cfg_init_pipeline_fn_t)(OBJ_cfg_t *cfg,  OBJ_pipeline_t *pl);
//finish up, the object now exists as a configuration
typedef bool (*OBJ_cfg_finish_fn_t)(OBJ_cfg_t *cfg);

typedef struct WS_OBJ_config_vtable
{
	OBJ_cfg_init_fn_t init_cfg;
	OBJ_cfg_add_setting_fn_t add_setting;
	OBJ_cfg_init_blueprint_fn_t init_blueprint;
	OBJ_cfg_init_pipeline_fn_t init_pipeline;
	OBJ_cfg_finish_fn_t finish_cfg;
}OBJ_cfg_vt_t;

struct WS_OBJ_config
{
	OBJ_cfg_vt_t *vt;
};

#endif