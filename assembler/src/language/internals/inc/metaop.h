#ifndef __METAOP_HEADER__
#define __METAOP_HEADER__

#include "commons.h"
#include <stdlib.h>
#include "parser.h"


typedef enum metaop_id
{
	MOP_UNKNOWN,
	MOP_I64,
	MOP_I32,
	MOP_I16,
	MOP_I8,
	MOP_FLOAT,
	MOP_DOUBLE,
	MOP_ALIGN,
	MOP_STRING,
	MOP_MEM,
	MOP_PTR,

	MOP_PUB,
	MOP_DEFINE,
	MOP_INCLUDE
}mop_id_t;

typedef enum mop_type
{
	MOP_TYPE_UNKNOWN,
	MOP_TYPE_DEFINE_DATA,
	MOP_TYPE_DEFINE_CONFIG,
}mop_type_t;

typedef struct data_holder
{

	uint64_t *words;
	size_t words_len;
}data_holder_t;

typedef enum config_type
{
	MOP_CONFIG_TYPE_INCLUDE,
	MOP_CONFIG_TYPE_PUBLIC,
	MOP_CONFIG_TYPE_DEFINE,

}config_type_t;


typedef struct config_include
{
	char *file_include;
	int id;
}config_include_t;

typedef struct config_public
{
	char *key_string;
	int count;
}config_public_t;

typedef struct config_define
{
	char *key_imply;
}config_define_t;


typedef struct config
{
	config_type_t type;

	union
	{
		config_include_t inc;
		config_define_t def;
		config_public_t pub;
	} config;
}config_t;

typedef struct metaop
{
	mop_id_t mop;
	mop_type_t type;


	char *mop_id;

	parse_node_t **expressions;
	size_t expressions_len;


	union
	{
		data_holder_t data;
		config_t config;
	}
	holder;

}mop_t;



void print_mop(mop_t *mop);
mop_t create_mop(parse_node_t *node);
mop_id_t get_mop_code(char *keyword);

data_holder_t decode_string(parse_node_t *head);
data_holder_t decode_integer(parse_node_t *head);



data_holder_t create_data_holder(parse_node_t *node);



config_public_t create_config_public(parse_node_t *node);
config_t create_config(parse_node_t *node, config_type_t type);





#endif