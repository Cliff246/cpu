#include "SIM_object.h"
#include "OBJ_bundle.h"
#include "OBJ_message.h"

#include "OBJ_message.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>


SIM_object_t SIM_object_init(SIM_device_t *device)
{

	OBJ_handle_t hnd = {};

	SIM_object_t obj =
	{

		.mod_handle = device->mod,
		.obj_handle = hnd,	
		.port = SIM_port_init(),
	};


	return obj;
}

void SIM_object_read(SIM_object_t *object, OBJ_bundle_t *bnd)
{

	assert(object);
	assert(bnd);
	assert(object->context && "object has null context");

	assert(object->pipeline.read != NULL && "object must have defined read");
	object->pipeline.read(&object->obj_handle, object->context, bnd);

}

void SIM_object_update(SIM_object_t *object)
{
	assert(object);
	assert(object->pipeline.update != NULL && "object must have defined update");
	assert(object->locals && "Object has null locals");
	assert(object->context && "object has null context");
	object->pipeline.update(&object->obj_handle, object->context, object->locals);
}

void SIM_object_write(SIM_object_t *object, OBJ_bundle_t *bnd)
{
	assert(object);
	assert(bnd);
	assert(object->context && "object has null context");
	assert(object->pipeline.write != NULL && "object must have defined write");
	object->pipeline.write(&object->obj_handle, object->context, bnd);
}
