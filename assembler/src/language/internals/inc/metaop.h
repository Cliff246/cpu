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
	MOP_U64,
	MOP_U32,
	MOP_U16,
	MOP_U8,
	MOP_FLOAT,
	MOP_DOUBLE,
	MOP_ALIGN,
	MOP_STRING,
	MOP_MEM,
	MOP_PTR,


}mop_id_t;

typedef enum mop_type
{
	MOP_TYPE_UNKNOWN,
	MOP_TYPE_DEFINE_DATA,
}mop_type_t;

typedef struct data_holder
{

	uint64_t *words;
	size_t words_len;
}data_holder_t;





typedef struct metaop
{
	mop_id_t mop;
	mop_type_t type;


	char *mop_keyword;

	parse_node_t **expressions;
	size_t expressions_len;

	data_holder_t data;

}mop_t;



void print_mop(mop_t *mop);
mop_t create_mop(parse_node_t *node);
mop_id_t get_mop_code(char *keyword);

data_holder_t decode_string(parse_node_t *head);
data_holder_t decode_integer(parse_node_t *head);



data_holder_t create_data_holder(parse_node_t *node);




#endif