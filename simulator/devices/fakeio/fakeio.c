#include "fakeio.h"

#include "WS_OBJ_flag.h"
#include "WS_MOD_description.h"
#include "hashmap.h"
#include "fakeio_version.h"
#include <stdlib.h>
#include <assert.h>
#include "fakeio_device_config_setting.h"

static const MOD_vtable_t vtable =
{

};

static MOD_description_t fakeio_desc =
{
	.id = 0,
	.version = SIM_DEV_FAKEIO_VERSION,

	.dev_name = "fakeio",
	.dev_typeclass = "mmio",
	.dl_name = "sim_dev_fakeio",

	.vtable = &vtable,
	.extra = NULL,

};


const MOD_description_t *MOD_get_dev_desc(void)
{
	static bool initialized = false;

	if(initialized == false)
	{
		
		//p_hashtable_t hashtable = new_hash_table(DEVICE_FAKEIO_CMD_OPTIONS_COUNT + WS_DEV_FLAG_DEFAULT_COUNT, WS_cmd_producer_free);

		//for(int i = 0; i < DEVICE_FAKEIO_CMD_OPTIONS_COUNT; ++i)
		{

			//WS_dev_flag_producer_t *producer = WS_cmd_flag_producer_create(device_fakeio_producer_names[i], device_fakeio_producer_types[i], device_fakeio_producer_functions[i]);

			//addto_hash_table(hashtable, device_fakeio_producer_names[i], producer);

		}

		//print_hash_table(hashtable);
		//fakeio_desc.flag_table = hashtable;
		//initialized = true;
	}


	return &fakeio_desc;
}


/*
void *device_fakeio_init(device_t *dev)
{
	assert(dev);
	dev_fakeio_t *fakeio = calloc(1, sizeof(dev_fakeio_t));
	//dev->has_address = true;
	fakeio->button_address = 0;
	fakeio->button_value = 0;
	fakeio->start = 0;
	fakeio->length = 0;
	assert(fakeio);
	return fakeio;
}

void device_fakeio_update(device_t *dev)
{
	dev_fakeio_t *fakeio = (dev_fakeio_t *)dev->ptr;

	if(fakeio->button_value > 0)
	{
		printf("%s\n", fakeio->fakeio_buffer);
		fakeio->button_value = 0;
	}
}

bool device_fakeio_read(WS_dev_t *dev, WS_dev_msg_t *msg)
{
	dev_fakeio_t *fakeio = dev->ptr;
	if(msg == NULL)
	{

	}
	else
	{
		dev_msg_type_t type = WS_get_device_message_type(msg);
		if(type == DEVMSG_WRITE)
		{
			if(msg->address == fakeio->button_address)
			{
				fakeio->button_value = msg->value;
			}
			else
			{
				if(msg->address >= fakeio->start && msg->address < fakeio->length + fakeio->start)
				{
					fakeio->fakeio_buffer[msg->address % FAKEIO_BUFFER_SIZE] = msg->value;

				}
				else
				{
					assert(0 && "failed to have valid address space");
				}

			}

		}
		else if (type == DEVMSG_READ_SEND)
		{
			if(fakeio->msg == NULL)
			{
				fakeio->msg = msg;
				return true;

			}
			else
			{
				return false;
			}
		}
	}

	return true;
}

bool device_fakeio_send(WS_dev_t *dev, WS_dev_msg_t **msg)
{
	dev_fakeio_t *fakeio = dev->ptr;

	if(fakeio->msg != NULL)
	{

		dev_msg_t *current = fakeio->msg;
		//get the read address
		uint64_t read_address = WS_get_device_message_address(current);
		uint64_t correct_read = read_address -  fakeio->start;

		int64_t respond = fakeio->fakeio_buffer[correct_read];
		//the response a bit brittle
		WS_device_message_respond(current, respond);
		fakeio->msg = NULL;
		*msg = current;
		return true;
	}
	else
	{
		return false;

	}

}

void device_fakeio_print(device_t *dev)
{
	dev_fakeio_t *fakeio = dev->ptr;



}


void device_fakeio_commit(WS_dev_t *dev)
{
	dev_fakeio_t *fakeio = dev->ptr;

	//dev->has_address = true;
}

*/
