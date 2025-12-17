#include "wcpu.h"
#include "device.h"
#include "device_description.h"
#include "device_vtable.h"
#include "wcpu_device_config_setting.h"
#include "device_command_impl.h"
#include "core.h"
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>


static const WS_dev_vtable_t vtable =
{

	.init = device_wcpu_init,
	.print = device_wcpu_print,
	.read = device_wcpu_read,
	.send = device_wcpu_send,
	.update = device_wcpu_update,
	.stringfy = device_wcpu_stringfy,
};

static WS_dev_desc_t wcpu_emu_desc =
{
	.id = 0,
	.version = 0,

	.dev_name = "wcpu_emu",
	.dev_typeclass = "cpu",
	.dl_name = "sim_dev_wcpu_emu",
	.vtable = &vtable,
	.extra = NULL,
};

const WS_dev_desc_t *WS_get_dev_desc(void)
{
	static bool initialized = false;

	if(initialized == false)
	{
		p_hashtable_t hashtable = new_hash_table(0, WS_cmd_producer_free);


		wcpu_emu_desc.flag_table = hashtable;
		initialized = true;
	}


	return &wcpu_emu_desc;
}




WS_dev_cmd_t *device_wcpu_stringfy(toklex_t *tl)
{

}



//this is temporary so we can send the read sendback request to the core
static bool wcpu_send_to_core(core_t *core, dev_msg_t *msg)
{
	assert(core != NULL && "core cannot be null when sending to it");
	assert(msg != NULL && "message when sent to core cannot be null");



	dev_msg_type_t type = get_device_message_type(msg);
	//read
	if(type == DEVMSG_READ_RESPOND)
	{
		//the issued instruction waiting... this is very bad code
		//rewrite this
		if(core->core_io.issued == true)
		{
			//this is fuckled (hehe fuckled) rewrite this
			int64_t value = msg->value;
			core->core_io.responded = true;
			core->core_io.value = value;
			return true;
		}
		//if the core has not issued anything to readback from it's not implemented yet
		else
		{
			print_device_message(msg);
			printf("\n");
			assert(false && "core has nothing to read back from");
		}
	}
	//write somehow or anything else sent through message stream
	else
	{
		assert(false &&  "cannot have anything but readbacks come to core right now");
	}

	return false;
}

void *device_wcpu_init(device_t *device)
{
	assert(device);
	dev_wcpu_t *cpu = calloc(1, sizeof(dev_wcpu_t));
	assert(cpu != NULL && "malloc cannot fail");
	cpu->core = wcpu_core_generate();
	assert(cpu->core);
	return cpu;
}





//very basic send to core. will change on multicore implementation
void device_wcpu_update(device_t *dev)
{
	assert(dev != NULL && "device cannot be null");
	dev_wcpu_t *wcpu = (dev_wcpu_t *)dev->ptr;

	core_t *core = wcpu->core;


	//if a message has come in handle it
	if(wcpu->has_in == true)
	{
		dev_msg_t *in_msg = wcpu->current_msg_in;
		//shit idk, error out
		if(wcpu_send_to_core(core, in_msg) == false)
		{
			assert(false && "cannot fail send to core");
		}


		//rest state after, if held by someone else should not free
		device_message_release(&in_msg);


		//release in the wcpu
		wcpu->current_msg_in = NULL;
		wcpu->has_in = false;
		wcpu->sent = false;

	}


	//do core update
	wcpu_core_update(core);

	//handle the output case
	if(core->core_io.issued == true && wcpu->has_out == false && wcpu->sent == false)
	{

		dev_msg_type_t msg_type = (core->core_io.type == CORE_IO_READ)? DEVMSG_READ_SEND : DEVMSG_WRITE;

		dev_msg_t *msg_out = device_message_create(&wcpu_emu_desc, dev->id, -1, msg_type, core->core_io.address, core->core_io.value);
		assert(msg_out);
		wcpu->current_msg_out = msg_out;
		wcpu->has_out = true;
	}



}

bool device_wcpu_read(device_t *dev, dev_msg_t *msg)
{
	assert(dev != NULL && "device cannot be null");
	dev_wcpu_t *wcpu = (dev_wcpu_t *)dev->ptr;

	core_t *core = wcpu->core;
	assert(core != NULL && "core cannot be null");

	if(msg == NULL)
	{
		//msg sent to me is null
		//so i guess skip it
		wcpu->has_in = false;
		wcpu->current_msg_in = NULL;
		return false;
	}
	wcpu->current_msg_in = msg;
	device_message_consume(&wcpu->current_msg_in);

	wcpu->has_in = true;
	return true;



}


dev_msg_t *device_wcpu_send(device_t *dev)
{
	assert(dev != NULL && "device cannot be null");
	dev_wcpu_t *wcpu = (dev_wcpu_t *)dev->ptr;

	if(wcpu->has_out == true && wcpu->sent == false)
	{
		dev_msg_t *msg = wcpu->current_msg_out;

		assert(msg != NULL && "msg cannot be null when outputting");

		wcpu->has_out = false;
		wcpu->sent = true;
		return msg;

	}
	else
	{
		return NULL;
	}





}



void device_wcpu_print(device_t *dev)
{
	dev_wcpu_t *wcpu = (dev_wcpu_t *)dev->ptr;


	printf("wcpu:\n");
	printf("	: msg_in: %d\n", wcpu->has_in);
	printf("	: msg_out: %d\n", wcpu->has_out);

	if(wcpu->has_in)
	{
		printf("\tin: ");
		print_device_message(wcpu->current_msg_in);
		printf("\n");
	}

	if(wcpu->has_out)
	{
		printf("\tout: ");
		print_device_message(wcpu->current_msg_out);
		printf("\n");
	}

}


void device_wcpu_cmd(device_t *dev, device_command_t *cmd)
{
	assert(0);
}