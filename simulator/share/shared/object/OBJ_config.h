#ifndef __WS_OBJ_CONFIG_HEADER__
#define __WS_OBJ_CONFIG_HEADER__

#include <stdbool.h>
#include <stdint.h>

typedef struct WS_OBJ_config_vtable
{

}OBJ_cfg_vt_t;

typedef struct WS_OBJ_config
{
	OBJ_cfg_vt_t *vt;
}OBJ_cfg_t;

#endif