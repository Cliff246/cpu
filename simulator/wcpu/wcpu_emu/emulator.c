#include "emulator.h"
#include "device_mailbox.h"
#include "device_message.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

emulator_t *emulator_generate(emuconfig_t *config)
{
	assert(config);
	emulator_t *emu = calloc(1, sizeof(emulator_t));
	assert(emu);
	if(emu == NULL)
	{
		perror("emulator was not created");
		exit(EXIT_FAILURE);
	}

	emu->config = config;

	emu->device_list = calloc(config->settings_index, sizeof(device_t *));
	assert(emu->device_list);
	if(emu->device_list == NULL)
	{
		perror("could not create device list");
		exit(EXIT_FAILURE);
	}

	emu->device_count = config->settings_index;
	//printf("%d\n", sizeof(emu_dev_slot_t));
	//printf("used %d\n", emu->device_count);

	emu->device_slots = calloc(emu->device_count, sizeof(emu_dev_slot_t));
	assert(emu->device_slots);
	//add stable slots
	emu->stable_slots = 0;


	for(int i = 0; i < emu->device_count; ++i)
	{
		device_t *dev = device_generate(emu->config->settings[i]);
		assert(dev);
		dev->device_id = i;
		if(get_device_changed(dev))
		{
			dev->flags[DEVICE_FLAG_TYPE_INTERNAL_CHANGED] = false;
		}

		emu_dev_slot_t slot =
		{
			.device_index = i,
			.device_id = dev->device_id,
			.address_start = dev->address_range_start,
			.address_length = dev->address_range_length
		};



		if(get_device_has_address(dev))
		{
			emu->device_slots[emu->stable_slots++] = slot;
		}
		emu->device_list[i] = dev;
	}
	//sort the slots of the emulator
	emulator_sort_slots(emu);
	return emu;
}

//true is overlap and false is no overlap
static bool cmp_overlap(size_t addr1, size_t addr2, size_t addr1_size, size_t addr2_size)
{
	size_t end1 = addr1 + addr1_size;
	size_t end2 = addr2 + addr2_size;

	return (bool)((end1 > addr2) && (end2 > addr1));
}

static void swap_slots(emulator_t *emu, size_t i, size_t j)
{

	emu_dev_slot_t temp = emu->device_slots[i];
	emu->device_slots[i] = emu->device_slots[j];
	emu->device_slots[j] = temp;
}

static size_t partition(emulator_t *emu, size_t lo, size_t hi)
{
	assert(emu);
	size_t pivot = emu->device_slots[hi].address_start;

	size_t i = lo;

	for(size_t j = lo; j <= (hi - 1); ++j)
	{
		if(emu->device_slots[j].address_start < pivot)
		{
            swap_slots(emu, i, j);
            i++;
		}
	}

    swap_slots(emu, i, hi);
    return i;
}

static void quicksort_emulator(emulator_t *emulator, size_t lo, size_t hi)
{
	if(lo >= hi) return;
	size_t pivot = partition(emulator, lo, hi);
	if(pivot > 0)
		quicksort_emulator(emulator, lo, pivot - 1);
	quicksort_emulator(emulator, pivot + 1, hi);
}

static void emulator_sort_slots(emulator_t *emu)
{
	if (emu->device_count == 0)
        return;

    quicksort_emulator(emu, 0, emu->stable_slots - 1);
 	for (size_t i = 0; i + 1 < emu->stable_slots; i++)
    {
        emu_dev_slot_t *A = &emu->device_slots[i];
        emu_dev_slot_t *B = &emu->device_slots[i+1];

        if (cmp_overlap(A->address_start, B->address_start,
                        A->address_length, B->address_length))
        {
            fprintf(stderr, "Address region overlap between device %zu and device %zu\n",
                    i, i+1);
            exit(1);
        }
		size_t start_a = A->address_start;
	   	size_t start_b = B->address_start;

		size_t length_a = A->address_length;
		size_t length_b = A->address_length;

		//printf("%lu %lu %lu %lu \n", start_a, start_b, length_a, length_b);
    }
}





static int emulator_device_address_search(emulator_t *emulator, size_t address)
{
	size_t low = 0;
	size_t high = emulator->stable_slots - 1;
	size_t mid = 0;
	while(low <= high)
	{
		mid = low + (high - low) / 2;
		emu_dev_slot_t *slot =  &emulator->device_slots[mid];
		size_t start = slot->address_start;
		size_t end = slot->address_length + start;
		//printf("%d %d\n", start, end);
		if(address < start)
		{
			high = mid - 1;

		}
		else if(address >= end)
		{
			low = mid + 1;
		}
		else
		{
			return mid;

		}
	}
	return -1;
}


//TODO
bool emulator_get_device_from_address(emulator_t *emulator, device_t **dev, size_t address)
{
	assert(emulator != NULL && "emulator cannot be null");
	assert(dev != NULL && "cannot give back nothing");
	//emulator_print_slots(emulator);
	//printf("address %d\n", address);

	int index = emulator_device_address_search(emulator, address);
	if(index == -1)
	{
		printf("no slots found\n");
		assert(0);
	}
	emu_dev_slot_t slot = emulator->device_slots[index];
	//printf("index %d %d %d %d \n", index, slot.address_start,slot.address_length,  address);

	if(address >= slot.address_start && address < slot.address_start + slot.address_length)
	{
		*dev = emulator->device_list[slot.device_index];
		return true;
	}

	return false;
}

device_t *emulator_get_device_from_id(emulator_t *emu, dev_id_t devid)
{
	assert(emu);
	assert(devid >= 0 && "device_id can never when searching == -1");
	assert(devid < emu->device_count && "device id can never be over device count");
	int index = (int)devid;
	device_t *dev = emu->device_list[index];
	return dev;
}




void emulator_update(emulator_t *emu)
{

	assert(emu);
	//printf("\nREAD STAGE\n\n");
	for(int ia= 0; ia < emu->device_count; ++ia)
	{

		device_t *dev = emu->device_list[ia];
		assert(dev != NULL && "device update found null device");

		dev_mailbox_t *mb = get_device_mailbox(dev);
		//device_mailbox_print(mb);
		dev_msg_t *get_msg;
		bool mailbox_has = device_mailbox_get(mb, &get_msg);

		//fill the msg_arg
		if(mailbox_has == true)
		{
			device_read(dev, get_msg);

		}
		//printf("\n");

		//device_print(dev);

	}

	//printf("\nUPDATE STAGE\n\n");

	for(int ib = 0; ib < emu->device_count; ++ib)
	{
		device_t *dev = emu->device_list[ib];
		assert(dev != NULL && "device update found null device");
		device_update(dev);
		//printf("\n");

		//device_print(dev);

		// update step
		//send out step
	}
	//printf("\nWRITE STAGE\n\n");

	for(int ic = 0; ic < emu->device_count; ++ic)
	{
		device_t *dev = emu->device_list[ic];
		assert(dev != NULL && "device update found null device");
		//printf("\n");
		dev_msg_t *set_msg = device_send(dev);


		//distrubute the message around
		if(set_msg != NULL)
		{

			bool has_dst = get_device_message_has_dst(set_msg);


			//check if it's on sendback or on release
			dev_msg_type_t type = get_device_message_type(set_msg);

			//check if it's a response


			device_t *dst_device;
			//print_device_message(set_msg);
			dev_id_t dst = -1;
			//print_device_message(set_msg);
			//printf("\n");
			if(type == DEVMSG_READ_RESPOND)
			{
				dst = get_device_message_src_id(set_msg);
				dst_device = emulator_get_device_from_id(emu, dst);

			}

			else if(has_dst)
			{
				//printf("has dest %d %d\n", has_dst, set_msg->dst_id);
				dst = get_device_message_dst_id(set_msg);
				dst_device = emulator_get_device_from_id(emu, dst);

			}
			//get dest from address
			else
			{

				size_t address = (size_t)get_device_message_address(set_msg);

				bool found_device =  emulator_get_device_from_address(emu, &dst_device, address);
				//checks if in that devices range
				if(found_device == false)
				{
					assert(0 && "cannot address out of range");
				}
				//should be safe.

			}

			//assume this is true
			dev_mailbox_t *dst_mb = get_device_mailbox(dst_device);
			bool could_put = device_mailbox_put(dst_mb, set_msg);
			assert(could_put != false && "could put must never be false");
		}
		//device_print(dev);

	}

}

static void print_slot(emu_dev_slot_t *slot, int index)
{
	printf("slot:%d, start:%d, stop:%d, devid:%d, dev_index:%d\n", index, slot->address_start, slot->address_length, slot->device_id, slot->device_index);
}

void emulator_print_slots(emulator_t *emu)
{
	assert(emu);
	for(int i = 0; i < emu->stable_slots; ++i)
	{
		print_slot(&emu->device_slots[i], i);
	}
}