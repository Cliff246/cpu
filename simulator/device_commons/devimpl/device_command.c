#include "device_command.h"
#include "commons.h"
#include "device_commons.h"
#include "device_command_impl.h"
#include "hashmap.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>




//assumes a fully valid
bool WS_device_cmd(WS_dev_t *device, WS_dev_cmd_t *cmd)
{
	assert(device && cmd);
	assert(device->desc == cmd->type);
	assert(cmd->collection);
	const WS_dev_desc_t *desc = device->desc;

	assert(desc->flag_table && "must have a valid flag hashtable in a device description");

	for(int i = 0; i < cmd->collection->flags_count; ++i)
	{
		WS_dev_cmd_flag_t *flag = cmd->collection->flags[i];
		char *flag_string = flag->flag;
		assert(flag_string && "all flags must have strings");
		printf("<flag string: %s> %d\n", flag_string);
		print_hash_table(desc->flag_table);
		WS_dev_cmd_flag_producer_t *producer = (WS_dev_cmd_flag_producer_t *)getdata_from_hash_table(desc->flag_table, flag_string);
		WS_cmd_producer_print(producer);
		printf("device name %s\n", device->desc->dev_name);
		assert(producer != NULL && "cannot use flag not in description flag table");

		assert(producer->expect == flag->type && "producer must be the same type as flag type");

		if(strcmp(flag_string, producer->id) != 0)
		{
			fprintf(stderr, "flag string: %s does not match producer: %s\n", flag_string, producer->id);
			assert(0);
		}
		assert(producer->fn && "must have valid producer function applied");
		bool allowed = producer->fn(device, &cmd->collection->flags[i]);

		assert(allowed && "producer for a flag must always be allowed");



	}

	//for now this will be changed later
	return true;

}

void WS_cmd_producer_print(WS_dev_cmd_flag_producer_t *producer)
{
	printf("producer:%s fn:%p expects:%d\n", producer->id, producer->fn, producer->expect);
}

void WS_device_cmd_free(WS_dev_cmd_t *cmd)
{
	assert(cmd);

	for(int i = 0; i < cmd->collection->flags_count; ++i)
	{
		WS_dev_cmd_flag_t *flag = &cmd->collection->flags[i];


		free(flag->flag);
		free(flag->value);
	}
	free(cmd);
}

void WS_cmd_producer_free(void *producer)
{
	WS_dev_cmd_flag_producer_t *typed = (WS_dev_cmd_flag_producer_t *)producer;


	free(typed->id);
	free(typed);
}

WS_dev_cmd_flag_producer_t *WS_cmd_flag_producer_create(char *string, WS_dev_cmd_flag_type_t type, WS_dev_cmd_flag_apply_fn fn)
{
	assert(string);
	assert(type != WS_DEV_CMD_FLAG_TYPE_UNKNOWN);
	assert(fn != NULL);

	WS_dev_cmd_flag_producer_t *producer = calloc(1, sizeof(WS_dev_cmd_flag_producer_t));
	assert(producer);
	producer->fn = fn;
	producer->id = strdup(string);
	assert(producer->id);
	producer->expect = type;

	return producer;

}

static void WS_cmd_collection_append(WS_dev_cmd_collection_t *col, WS_dev_cmd_flag_t *flag)
{
	col->flags = realloc_safe(col->flags, col->flags_count + 1, sizeof(WS_dev_cmd_flag_t*));
	col->flags[col->flags_count++] = flag;
	return;
}

WS_dev_cmd_flag_t *WS_cmd_flag_create(tok_t *key, tok_t *value)
{

	WS_dev_cmd_flag_t *flag = calloc(1, sizeof(WS_dev_cmd_flag_t));
	assert(flag);

	flag->flag = strdup(key->token);
	WS_dev_cmd_flag_value_t *flag_value = calloc(1, sizeof(WS_dev_cmd_flag_value_t));

	printf("produce token %s %d\n", value->token, value->type);
	switch(value->type)
	{
		case TOK_INT:
			(*flag_value).INT = atoi(value->token);
			flag->type = WS_DEV_CMD_FLAG_TYPE_INT;
			break;

		case TOK_STRING:
			(*flag_value).STRING = strdup(value->token);
			flag->type = WS_DEV_CMD_FLAG_TYPE_STRING;

			break;

		default:

			break;


	}
	flag->value = flag_value;


	return flag;
}

WS_dev_cmd_collection_t *WS_cmd_collection_create(toklex_t *tl)
{
	WS_dev_cmd_collection_t *col = calloc(1, sizeof(WS_dev_cmd_collection_t));

	assert(col);
	print_toklex(tl);
	for(int i = 0; i < tl->tcount - 1; i += 2)
	{
		tok_t *t1 = &tl->tokens[i];
		tok_t *t2 = &tl->tokens[i + 1];
		//printf("%d %d\n", i, tl->tcount);
		WS_dev_cmd_flag_t *flag = WS_cmd_flag_create(t1, t2);
		WS_cmd_collection_append(col, flag);
	}


	return col;
}

/*
device_config_setting_vtable_t device_config_setting_vtable_array[] =
{
	[DEVICE_INVAL] = {
		NULL,
		NULL,
	},

	[DEVICE_WCPU] = {
		.init = device_init_config_setting_wcpu,
		.free = device_free_config_setting_wcpu,
	},

	[DEVICE_FAKEIO] = {
		.init = device_init_config_setting_fakeio,
		.free = device_free_config_setting_fakeio,
	},
	[DEVICE_RAM] = {
		.init = device_init_config_setting_ram,
		.free = device_free_config_setting_ram,
	},
};


device_command_t *device_command_generate(device_type_t type)
{
	assert(type != DEVICE_INVAL && "device is not invalid");
	device_command_t *command = calloc(1, sizeof(device_command_t));

	assert(command != NULL);

	command->type = type;
	command->setting = device_init_config_setting(type);

	assert(command->setting.ptr != NULL);
	command->used = true;
	return command;
}

void device_command_free(device_command_t *command)
{
	//be super safe
	assert(command != NULL);
	//super duper safe
	assert(command->type != DEVICE_INVAL);
	//
	assert(command->used == false);

	device_free_config_setting(command->setting, command->type);

	free(command);

}

device_config_setting_ptr_t device_init_config_setting(device_type_t type)
{

	assert(type != DEVICE_INVAL && "device is not invalid");
	assert(device_config_setting_vtable_array[type].init != NULL);
	return device_config_setting_vtable_array[type].init(type);
}

void device_free_config_setting(device_config_setting_ptr_t ptr, device_type_t type)
{
	assert(type != DEVICE_INVAL && "device is not invalid");

	assert(ptr.ptr);

	assert(device_config_setting_vtable_array[type].free != NULL);

	device_config_setting_vtable_array[type].free(ptr);

}

*/