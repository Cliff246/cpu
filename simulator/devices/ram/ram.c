#include "MOD_description.h"
#include "ram.h"
#include "ram_device_config_setting.h"
#include "ram_version.h"
#include "hashmap.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <errno.h>
#include <assert.h>




static MOD_description_t ram_desc =
{
	.id = 0,
	.version = STR(SIM_RAM_DEV_VERSION),

	.dev_name = "ram",
	.dev_typeclass = "mmio",
	.dl_name = "sim_dev_ram",

};


const MOD_description_t *MOD_get_dev_desc(void)
{
	return &ram_desc;
}



