#ifndef __MODULE_VTABLE_HEADER__
#define __MODULE_VTABLE_HEADER__

#include <stdbool.h>
#include <stdint.h>
#include "monolith_instance.h"
#include "monolith_tag.h"

typedef void (*MOD_update_fn)(MLTH_instance_t *inst);
typedef MLTH_instance_t *(MOD_init_fn)(void);
typedef void (*MOD_pkt_get_fn)(MLTH_instance_t *inst, int data , int bid);
typedef int (*MOD_pkt_set_fn)(MLTH_instance_t *inst, int bid);
typedef void (*MOD_tag_get_fn)(MLTH_instance_t *inst, int data);
typedef MLTH_tag_t (*MOD_tag_set_fn)(MLTH_instance_t *inst);



typedef struct WS_MOD_vtable
{

}MOD_vtable_t;

#endif