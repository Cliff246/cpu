#include "MOD_description.h"
#include "MOD_symmap.h"
#include "OBJ_handle.h"
#include "ram.h"
#include "ram_device_config_setting.h"
#include "ram_version.h"
#include "hashmap.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <errno.h>
#include <assert.h>


static MOD_symmap_t ram_map =
{
	.size = 0,
	.symbols = {}
};

static MOD_description_t ram_desc =
{
	.id = 0,
	.version = STR(SIM_RAM_DEV_VERSION),
	.symmap = &ram_map,
	.dev_name = "ram",
	.dev_typeclass = "mmio",
	.dl_name = "sim_dev_ram",
};


const MOD_description_t *MOD_get_dev_desc(void)
{
	return &ram_desc;
}








/*


	idea for ram





	ram sets up what it know's




*/

