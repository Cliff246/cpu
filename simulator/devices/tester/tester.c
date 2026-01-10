#include "tester.h"

#include "MOD_description.h"




static MOD_description_t tester_desc =
{
	.id = 0,
	.version = 0,

	.dev_name = "tester",
	.dev_typeclass = "tester",
	.dl_name = "module_tester",


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


	return &tester_desc;
}

