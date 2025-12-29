#include "fakeio.h"

#include "device_commons.h"
#include "device_description.h"
#include "device_vtable.h"
#include "hashmap.h"
#include "device_command_impl.h"
#include "fakeio_version.h"
#include <stdlib.h>
#include <assert.h>

static const WS_dev_vtable_t vtable =
{
	.stringfy = device_fakeio_stringfy,
	.init = device_fakeio_init,
	.print = device_fakeio_print,
	.read = device_fakeio_read,
	.send = device_fakeio_send,
	.update = device_fakeio_update,
	.cmd_commit = device_fakeio_commit,
};

static WS_dev_desc_t fakeio_desc =
{
	.id = 0,
	.version = SIM_DEV_FAKEIO_VERSION,

	.dev_name = "fakeio",
	.dev_typeclass = "mmio",
	.dl_name = "sim_dev_fakeio",

	.vtable = &vtable,
	.extra = NULL,

};


const WS_dev_desc_t *WS_get_dev_desc(void)
{
	static bool initialized = false;

	if(initialized == false)
	{
		p_hashtable_t hashtable = new_hash_table(DEVICE_FAKEIO_CMD_OPTIONS_COUNT, WS_cmd_producer_free);

		for(int i = 0; i < DEVICE_FAKEIO_CMD_OPTIONS_COUNT; ++i)
		{

			WS_dev_cmd_flag_producer_t *producer = WS_cmd_flag_producer_create(device_fakeio_producer_names[i], device_fakeio_producer_types[i], device_fakeio_producer_functions[i]);

			addto_hash_table(hashtable, device_fakeio_producer_names[i], producer);

		}

		//print_hash_table(hashtable);
		fakeio_desc.flag_table = hashtable;
		initialized = true;
	}


	return &fakeio_desc;
}



void *device_fakeio_init(device_t *dev)
{
	assert(dev);
	dev_fakeio_t *fakeio = calloc(1, sizeof(dev_fakeio_t));
	dev->has_address = true;

	assert(fakeio);
	return fakeio;
}

void device_fakeio_update(device_t *dev)
{

}

bool device_fakeio_read(WS_dev_t *dev, WS_dev_msg_t *msg)
{
	dev_fakeio_t *fakeio = dev->ptr;
	if(msg==NULL)
	{

	}
	else
	{
		dev_msg_type_t type = WS_get_device_message_type(msg);
		if(type == DEVMSG_WRITE)
		{
			fakeio->fakeio_buffer[msg->address % FAKEIO_BUFFER_SIZE] = msg->value;
		}
	}

	return true;
}

bool device_fakeio_send(WS_dev_t *dev, WS_dev_msg_t **msg)
{
	return false;
}

void device_fakeio_print(device_t *dev)
{

}

WS_dev_cmd_t *device_fakeio_stringfy(toklex_t *tl)
{

	WS_dev_cmd_t *cmd =  calloc(1, sizeof(WS_dev_cmd_t));

	cmd->type = &fakeio_desc;
	cmd->used = false;
	cmd->collection = WS_cmd_collection_create(tl);


	return cmd;
}

void device_fakeio_commit(WS_dev_t *dev)
{

}

