#ifndef __DEVICE_HEADER_
#define __DEVICE_HEADER__

#include "dev_wcpu.h"
#include "ram.h"

typedef enum device_type
{
	DEVICE_INVAL,
	DEVICE_WCPU,
	DEVICE_FAKEIO,
	DEVICE_DEBUGER,
	DEVICE_RAM,
}device_type_t;



typedef union device_option
{

}device_option_t;



typedef void (clock_fn)(device_t *device);
typedef bool (hook_address_fn)(device_t *device, int64_t hook);


typedef struct device
{
	device_type_t type;



	device_option_t ptr;


}device_t;




#endif