#ifndef __FAKEIO_HEADER__
#define __FAKEIO_HEADER__


#include <stdint.h>
#include "device.h"
#include "device_type_ptr.h"

typedef struct device_fakeio
{

}dev_fakeio_t;


device_t *device_fakeio_generate(void);
void device_fakeio_update(device_t *device);

#endif