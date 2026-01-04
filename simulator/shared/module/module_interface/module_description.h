#ifndef __MODULE_DESCRIPTION_HEADER__
#define __MODULE_DESCRIPTION_HEADER__

#include "object_vtable.h"
#include "object_tag.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef _WIN32
#  define DEVICE_EXPORT __declspec(dllexport)
#else
#  define DEVICE_EXPORT __attribute__((visibility("default")))
#endif





typedef struct WS_MOD_description
{
	int id;
	int version;
	const char *mod_name;
	const char *mod_typeclass;
	const char *dl_name;

	const OBJ_vtable_t *vtable;

	const struct
	{
		OBJ_tag_handle_t *handles;
	 	uint64_t size;
	}default_hnd;


	void *extra;
}MOD_description_t;


DEVICE_EXPORT const MOD_description_t *MOD_get_desc(void);
#endif