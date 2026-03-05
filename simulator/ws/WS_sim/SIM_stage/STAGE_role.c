#include "STAGE_role.h"

#include <stdint.h>
#include <string.h>

#define STAGE_ROLE_DEFINE(X, Y)

//this is fucking cringe
char *role_id_array[] =
{
	"device",
	"wire",
	"bus"
};

SIM_rid_t STAGE_get_rid(char *role)
{
	bool passed = false;

	const uint64_t check_size = sizeof(role_id_array) / sizeof(role_id_array[0]);

	for(uint64_t i = 0; i < check_size; ++i)
	{
		if(!strcmp(role, role_id_array[i]))
		{
			return i;
		}
	}
	return -1;
}


char *STAGE_get_str_rid(SIM_rid_t rid)
{
	const uint64_t check_size = sizeof(role_id_array) / sizeof(role_id_array[0]);

	if(rid < 0)
		return "UNKNOWN";
	if(rid > check_size)
		return "UNKNOWN";
	return role_id_array[rid];
}