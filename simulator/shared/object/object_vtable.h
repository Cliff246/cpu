#ifndef __OBJECT_VTABLE_HEADER__
#define __OBJECT_VTABLE_HEADER__

#include <stdbool.h>
#include <stdint.h>
#include "object_instance.h"
#include "object_tag.h"

typedef void (*OBJ_update_fn)(OBJ_instance_t *inst);
typedef OBJ_instance_t *(*OBJ_init_fn)(void);
typedef void (*OBJ_pkt_get_fn)(OBJ_instance_t *inst);
typedef int (*OBJ_pkt_set_fn)(OBJ_instance_t *inst);
typedef void (*OBJ_tag_get_fn)(OBJ_instance_t *inst);
typedef OBJ_tag_t (*OBJ_tag_set_fn)(OBJ_instance_t *inst);



typedef struct WS_OBJ_vtable
{
	OBJ_update_fn update;
	OBJ_init_fn init;
	OBJ_pkt_get_fn pkt_get;
	OBJ_pkt_set_fn pkt_set;
	OBJ_tag_get_fn tag_get;
	OBJ_tag_set_fn tag_set;
}OBJ_vtable_t;

#endif