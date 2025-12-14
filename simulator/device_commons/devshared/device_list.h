#ifndef __DEVICE_LIST_HEADER__
#define __DEVICE_LIST_HEADER__

#include "commons.h"

#define DEVICE_LIST(X)	\
	X(WCPU, wcpu)		\
	X(RAM, ram)			\
	X(FAKEIO, fakeio)	\



#define DEVICE_NAME_TYPE(name, Y) DEVICE_ ## name

#define DEVICE_NAME_STRUCT(X, name) name

#define DEVICE_TYPE_ENUM(X, Y) DEVICE_NAME_TYPE(X, Y),


typedef enum device_type
{
	DEVICE_INVAL,
	DEVICE_LIST(DEVICE_TYPE_ENUM)
}device_type_t;


typedef int dev_id_t;




#endif