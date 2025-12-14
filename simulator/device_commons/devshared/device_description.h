#ifndef __DEVICE_DESCRIPTION_HEADER__
#define __DEVICE_DESCRIPTION_HEADER__

#include "device_vtable.h"

#ifdef _WIN32
#  define DEVICE_EXPORT __declspec(dllexport)
#else
#  define DEVICE_EXPORT __attribute__((visibility("default")))
#endif

typedef struct device_description
{
	int id;
	int version;
	const char *dev_name;
	const char *dev_typeclass;
	const char *dl_name;

	const WS_dev_vtable_t *vtable;


	void *extra;
}WS_dev_desc_t;


DEVICE_EXPORT const WS_dev_desc_t *WS_get_dev_desc(void);
#endif