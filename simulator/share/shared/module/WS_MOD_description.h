#ifndef __MODULE_DESCRIPTION_HEADER__
#define __MODULE_DESCRIPTION_HEADER__

#include "WS_MOD_vtable.h"
#include "hashmap.h"

#ifdef _WIN32
#  define DEVICE_EXPORT __declspec(dllexport)
#else
#  define DEVICE_EXPORT __attribute__((visibility("default")))
#endif





typedef struct WS_MOD_description
{
	int id;
	int version;
	const char *dev_name;
	const char *dev_typeclass;
	const char *dl_name;

	const MOD_vtable_t *vtable;

	//contains elements of WS_dev_cmd_flag_producer_t
	p_hashtable_t flag_table;

	void *extra;

}MOD_description_t;



DEVICE_EXPORT const MOD_description_t *MOD_get_dev_desc(void);
#endif