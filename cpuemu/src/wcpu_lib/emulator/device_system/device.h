#ifndef __DEVICE_HEADER_
#define __DEVICE_HEADER__

#include "dev_wcpu.h"
#include "dev_ram.h"
#include "dev_fakeio.h"

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
	dev_wcpu_t *cpu;
	dev_ram_t *ram;
	dev_fakeio_t *fakeio;

}device_option_t;





typedef struct device
{
	device_type_t type;

	int error;

	device_option_t ptr;




}device_t;


device_t *create_device(device_type_t *type);

void step_device(device_t *device);



#endif