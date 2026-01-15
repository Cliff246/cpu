#include "SIM_simulator.h"
#include "IO_configure.h"
#include "SIM_graph.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

//TODO not done should create a simple graph unready


//advance packet forward if retired return true
//if not retired return false
SIM_simulator_t *SIM_simulator_init(void)
{
	SIM_simulator_t *sim = calloc(1, sizeof(SIM_simulator_t));
	return sim;
}

void SIM_simulator_update(SIM_simulator_t *sim)
{
	SIM_graph_update(sim->graph);


}

bool SIM_simulator_load_config(SIM_simulator_t *sim, WS_cfg_file_t *config)
{

}

void SIM_simulator_print_slots(SIM_simulator_t *sim)
{

}

void SIM_simulator_print_all_devices(SIM_simulator_t *sim)
{



}


/*
static bool cmp_overlap(size_t addr1, size_t addr2, size_t addr1_size, size_t addr2_size)
{
	size_t end1 = addr1 + addr1_size;
	size_t end2 = addr2 + addr2_size;

	return (bool)((end1 > addr2) && (end2 > addr1));
}

static void simulator_swap_slots(WS_simulator_t *sim, size_t i, size_t j)
{

	WS_simulator_bus_slot_t *temp = sim->bus_slot[i];
	sim->bus_slot[i] = sim->bus_slot[j];
	sim->bus_slot[j] = temp;
}



static size_t simulator_partition(WS_simulator_t *sim, size_t lo, size_t hi)
{
	assert(sim);
	size_t pivot = sim->bus_slot[hi]->start;

	size_t i = lo;

	for(size_t j = lo; j <= (hi - 1); ++j)
	{
		if(sim->bus_slot[j]->start < pivot)
		{
            simulator_swap_slots(sim, i, j);
            i++;
		}
	}

    simulator_swap_slots(sim, i, hi);
    return i;
}

static void simulator_quicksort(WS_simulator_t *sim, size_t lo, size_t hi)
{
	if(lo >= hi) return;
	size_t pivot = simulator_partition(sim, lo, hi);
	if(pivot > 0)
		simulator_quicksort(sim, lo, pivot - 1);
	simulator_quicksort(sim, pivot + 1, hi);
}

WS_dev_t *WS_simulator_get_device_from_id(WS_simulator_t *sim, WS_dev_id_t devid)
{
	int index = (int)devid;

	assert(devid >= 0 && "device_id can never when searching == -1");
	assert(devid < sim->dev_count && "device id can never be over device count");


	for(int i = 0; i < sim->dev_count; ++i)
	{
		if(sim->dev_list[i]->id == devid)
			return sim->dev_list[i];
	}
	assert(0);
}

bool WS_simulator_get_device_from_address(WS_simulator_t *sim, WS_dev_t **dev, size_t address)
{
	assert(sim!= NULL && "emulator cannot be null");
	assert(dev != NULL && "cannot give back nothing");
	//printf("address %d\n", address);
	if(sim->bus_slot_count <= 0)
	{
		return false;
	}
	int index = simulator_device_address_search(sim, address);
	if(index == -1)
	{
		printf("no slots found\n");
		assert(0);
	}
	WS_simulator_bus_slot_t *slot = sim->bus_slot[index];
	//printf("index %d %d %d %d \n", index, slot.address_start,slot.address_length,  address);

	if(address >= slot->start && address < slot->start + slot->len)
	{
		*dev = sim->dev_list[slot->dev_index];
		return true;
	}

	return false;
}

static void WS_simulator_sort_slots(WS_simulator_t *sim)
{
	if (sim->dev_count == 0)
        return;

    simulator_quicksort(sim, 0, sim->bus_slot_count - 1);
 	for (size_t i = 0; i + 1 < sim->bus_slot_count; i++)
    {
        WS_simulator_bus_slot_t *A = sim->bus_slot[i];
        WS_simulator_bus_slot_t *B = sim->bus_slot[i+1];

        if (cmp_overlap(A->start, B->start,
                        A->len, B->len))
        {
			WS_simulator_print_slots(sim);

            fprintf(stderr, "Address region overlap between device %zu and device %zu\n",
                    i, i+1);
            exit(1);
        }
		size_t start_a = A->start;
	   	size_t start_b = B->start;

		size_t length_a = A->len;
		size_t length_b = B->len;

		//printf("%lu %lu %lu %lu \n", start_a, start_b, length_a, length_b);
    }
}

WS_simulator_t *WS_simulator_init()
{
	WS_simulator_t *sim = calloc(1, sizeof(WS_simulator_t));
	assert(sim);

	return sim;
}

//add device to simulator
void WS_simulator_add_device(WS_simulator_t *sim, WS_dev_t *dev)
{
	assert(sim && "simulator must be found");
	assert(dev && "device must be created");
	sim->dev_list = realloc_safe(sim->dev_list, sim->dev_count + 1, sizeof(WS_dev_t *) );

	if(dev->has_address)
	{
		WS_simulator_bus_slot_t *slot =  WS_simulator_create_bus_slot(dev->address_range_start, dev->address_range_length,  sim->dev_count );

		sim->bus_slot = realloc_safe(sim->bus_slot, sim->bus_slot_count + 1, sizeof(WS_simulator_bus_slot_t *) );

		sim->bus_slot[sim->bus_slot_count++] = slot;
		WS_simulator_sort_slots(sim);
	}


	sim->dev_list[sim->dev_count++] = dev;



}

bool WS_simulator_load_config(WS_simulator_t *sim, WS_cfg_file_t *config)
{
	//printf("%d\n",config->module_container_list_count);
	for(int i = 0; i <config->size; ++i)
	{
		WS_cfg_hdr_t *header = config->headers[i];

		//printf("%s entry list count: %d\n", container->module->dev_desc->dl_name,container->entry_list_count);
		for(int x = 0; x < header->size; ++x)
		{

			device_t *device = WS_device_init(header->module->dev_desc, header->cmds[x]);

			WS_simulator_add_device(sim, device);
		}
	}
}


void WS_simulator_update(WS_simulator_t *sim)
{

	assert(sim);
	//printf("\nREAD STAGE\n\n");
	for(int ia= 0; ia < sim->dev_count; ++ia)
	{

		device_t *dev = sim->dev_list[ia];
		assert(dev != NULL && "device update found null device");

		//WS_device_mailbox_print(dev);
		dev_msg_t *get_msg;
		bool mailbox_has = WS_device_mailbox_pop(dev, &get_msg);

		//fill the msg_arg
		if(mailbox_has == true)
		{
			WS_device_read(dev, get_msg);

		}
		//printf("\n");
		//printf("%s\n", dev->desc->dev_name);
		//device_print(dev);

	}

	//printf("\nUPDATE STAGE\n\n");

	for(int ib = 0; ib < sim->dev_count; ++ib)
	{
		device_t *dev = sim->dev_list[ib];
		assert(sim != NULL && "device update found null device");
		WS_device_update(dev);
		//printf("\n");

		//device_print(dev);

		// update step
		//send out step
	}
	//printf("\nWRITE STAGE\n\n");
	//	WS_simulator_print_slots(sim);
//	printf("AFTER\n");

	for(int ic = 0; ic < sim->dev_count; ++ic)
	{
		device_t *dev = sim->dev_list[ic];
		assert(dev != NULL && "device update found null device");
		//printf("\n");
		WS_dev_msg_t *set_msg = NULL;
		bool has_msg = WS_device_send(dev, &set_msg);
		//printf("%d %p\n", has_msg, set_msg);
		//distrubute the message around
		if(has_msg )
		{
			//printf("message sent\n");
			bool has_dst = WS_get_device_message_has_dst(set_msg);


			//check if it's on sendback or on release
			dev_msg_type_t type = WS_get_device_message_type(set_msg);

			//check if it's a response

			device_t *dst_device;
			//print_device_message(set_msg);
			WS_dev_id_t dst = -1;
			//print_device_message(set_msg);
			//printf("\n");
			//printf("%d %d\n", type, has_dst);

			if(type == DEVMSG_READ_RESPOND)
			{
				dst = WS_get_device_message_src_id(set_msg);
				dst_device = WS_simulator_get_device_from_id(sim, dst);

			}

			else if(has_dst)
			{
				//printf("has dest %d %d\n", has_dst, set_msg->dst_id);
				dst = WS_get_device_message_dst_id(set_msg);
				dst_device = WS_simulator_get_device_from_id(sim, dst);
			}
			//get dest from address
			else
			{

				size_t address = (size_t)WS_get_device_message_address(set_msg);
				//printf("%d\n", address);
				bool found_device =  WS_simulator_get_device_from_address(sim, &dst_device, address);
				//checks if in that devices range
				if(found_device == false)
				{
					assert(0 && "cannot address out of range");
				}
				//should be safe.

			}

			//assume this is true
			bool could_put = WS_device_mailbox_put(dst_device, set_msg);
			assert(could_put != false && "could put must never be false");
		}
		//device_print(dev);

	}
}

void WS_simulator_print_slots(WS_simulator_t *sim)
{
	assert(sim);
	for(int i = 0; i < sim->bus_slot_count; ++i)
	{
		WS_simulator_print_bus_slot(sim->bus_slot[i], i);
	}
}


void WS_simulator_print_all_devices(WS_simulator_t *sim)
{
	printf("print all\n");
	for(int i = 0; i < sim->dev_count; ++i)
	{
		WS_device_print(sim->dev_list[i]);
	}
}


*/