#include "dev_wcpu.h"
#include "core.h"
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>


device_type_ptr_t device_wcpu_generate(device_t *device,emuconfig_dev_settings_t *settings)
{
	dev_wcpu_t *cpu = calloc(1, sizeof(dev_wcpu_t));
	assert(cpu != NULL && "malloc cannot fail");
	cpu->core = wcpu_core_generate();

	device_type_ptr_t ptr;
	ptr.wcpu = cpu;
	return ptr;

}


//very basic send to core. will change on multicore implementation
void device_wcpu_update(device_t *device)
{
	wcpu_core_update(device->device.wcpu->core);
}

void device_wcpu_read(device_t *dev, dev_msg_t *send)
{
	assert(dev != NULL && "device cannot be null");
		
	dev_wcpu_t *wcpu = dev->device.wcpu;
	
	core_t *core = wcpu->core;
	if(send)
	{

		//read 
		if(send->is_response == false)
		{
			if(core->core_io.issued == true)
			{
				//this is fuckled (hehe fuckled) rewrite this
				int64_t value = send->value; 
				core->core_io.responded = true;
				core->core_io.value = value;
				device_message_release(send);
			}
			else
			{

			}

		}	

	}
}


dev_msg_t *device_wcpu_send(device_t *dev)
{
	return NULL;
}
