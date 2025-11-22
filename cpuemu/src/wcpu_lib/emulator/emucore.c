#include "emucore.h"

#include <stdlib.h>
#include <stdio.h>

emulator_t *emulator_generate(emuconfig_t *config)
{

	emulator_t *emu = calloc(1, sizeof(emulator_t));

	if(emu == NULL)
	{
		perror("emulator was not created");
		exit(EXIT_FAILURE);
	}




	emu->config = config;

	emu->device_list = calloc(config->settings_index, sizeof(device_t *));

	if(emu->device_list == NULL)
	{
		perror("could not create device list");
		exit(EXIT_FAILURE);
	}

	emu->device_count = config->settings_index;
	printf("%d\n", sizeof(emu_dev_slot_t));
	printf("used %d\n", emu->device_count);

	emu->device_slots = calloc(emu->device_count, sizeof(emu_dev_slot_t));

	if(emu->device_slots == NULL)
	{
		perror("could not create device slots");
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < emu->device_count; ++i)
	{
		if(emu->config->settings[i].used == true)
		{
			device_t *dev = device_generate(&emu->config->settings[i]);
			emu_dev_slot_t slot =
			{
				.device_index = i,
				.device_id = dev->device_id,
				.address_start = dev->address_range_start,
				.address_length = dev->address_range_length
			};

			emu->device_slots[i] = slot;
			emu->device_list[i] = dev;


		}
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
	if(i == j)
	{
		perror("cannot swap the same address");
		exit(EXIT_FAILURE);
	}
	emu_dev_slot_t temp = emu->device_slots[i];
	emu->device_slots[i] = emu->device_slots[j];
	emu->device_slots[j] = temp;
}

static size_t partition(emulator_t *emu, size_t lo, size_t hi)
{
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
	printf("lo hi %d %d\n", lo, hi);
	size_t pivot = partition(emulator, lo, hi);
	if(pivot > 0)
		quicksort_emulator(emulator, lo, pivot - 1);
	quicksort_emulator(emulator, pivot + 1, hi);
}

static void emulator_sort_slots(emulator_t *emu)
{
	if (emu->device_count == 0)
        return;

    quicksort_emulator(emu, 0, emu->device_count - 1);
 	for (size_t i = 0; i + 1 < emu->device_count; i++)
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
		printf("%d %d %d %d\n", A->address_start, B->address_start, A->address_length, B->address_length);
    }
}





static int emulator_device_address_search(emulator_t *emulator, size_t address)
{
	size_t low = 0;
	size_t high = emulator->device_count - 1;
	size_t mid = 0;
	while(low <= high)
	{
		mid = low + (high - low) / 2;

		size_t mid_value = emulator->device_slots[mid].address_start;
		if(mid_value == address)
			return mid;
		if(mid_value < address)
			low = mid + 1;
		else
			high = mid - 1;
	}
	return mid;
}


device_t *emulator_device_by_address(emulator_t *emulator, size_t address)
{







}

//todo
void peek_system(char *cmd);