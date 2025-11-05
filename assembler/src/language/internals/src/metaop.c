#include "metaop.h"

#include "commons.h"
#include "lexer.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "entry.h"
#include "decoder.h"
#include "strtools.h"
#include <assert.h>


const char *mopids_as_strings[] =
{
	[MOP_UNKNOWN] = "inval",
	[MOP_I64] = "i64",
	[MOP_I32] = "i32",
	[MOP_I16] = "i16",
	[MOP_I8] = "i8",
	[MOP_U64] = "u64",
	[MOP_U32] = "u32",
	[MOP_U16] = "u16",
	[MOP_U8] = "u8",
	[MOP_FLOAT] = "float",
	[MOP_DOUBLE] = "double",
	[MOP_ALIGN] = "align",
	[MOP_STRING] = "string",
	[MOP_MEM] = "mem",
	[MOP_PTR] = "ptr",
};

static bool is_mop_id_integer(mop_id_t id)
{
	switch(id)
	{
		case MOP_I16:
			return true;
		case MOP_I32:
			return true;
		case MOP_I64:
			return true;
		case MOP_I8:
			return true;
		case MOP_U16:
			return true;
		case MOP_U32:
			return true;
		case MOP_U64:
			return true;
		case MOP_U8:
			return true;
		default:
			return false;
	}
}

static bool is_mop_id_string(mop_id_t id)
{
	if(id == MOP_STRING)
		return true;
	else
		return false;
}


data_holder_t decode_string(parse_node_t *head)
{
	size_t buffer_alloc = 100, buffer_size = 0;
	data_holder_t holder = {0};

	uint8_t *buffer = CALLOC(buffer_alloc, uint8_t);
	//printf("mop string\n");
	//the prefill into a long string
	for(int i = 0; i < head->child_count; ++i)
	{
		parse_node_t *child = head->children[i];
		if(child->kind == NODE_LITERAL)
		{
			if(child->tok->type == TOK_STRING)
			{
				int size = strlen(child->tok->lexeme);
				//fill in string
				for(int i = 1; i < size - 1; ++i)
				{

					if(buffer_size + 1 > buffer_alloc)
					{
						buffer = REALLOC(buffer, buffer_alloc *= 2, uint8_t);
					}
					buffer[buffer_size++] = child->tok->lexeme[i];
				}



			}
			else
			{
				printf("parse node is not a string %s\n", child->tok->lexeme);
			}
		}
	}

	if(buffer_alloc <= buffer_size + 1)
	{
		buffer = REALLOC(buffer, buffer_alloc *= 2, uint8_t);

	}
	buffer[buffer_size++] = 0;

	printf("buffer: %s\n", buffer);
	size_t mod = buffer_size % 8;
	size_t len = buffer_size / 8;
	if(mod > 0)
	{
		len++;
	}

	uint64_t *actual_data = CALLOC(len, uint64_t);

	memcpy(actual_data, buffer, buffer_size);

	free(buffer);
	holder.words = actual_data;
	holder.words_len = len;
	return holder;
}

data_holder_t decode_integer(parse_node_t *head)
{
	data_holder_t holder = {0};
	size_t word_size = head->child_count;
	holder.words = CALLOC(word_size, uint64_t);
	holder.words_len = word_size;

	for(int i = 0; i < word_size; ++i)
	{

		char *tok = head->children[i]->tok->lexeme;
		number_type_t type = get_number_type(tok);
		int64_t imm = 0;
		if(type == NUM_INT)
		{
			imm = atoi(tok);
		}
		if(type == NUM_HEX)
		{
			imm = convert_to_hex(tok);
		}
		if(type == NUM_OCT)
		{
			imm = convert_to_oct(tok);
		}

		holder.words[i] = imm;
	}

	return holder;

}

data_holder_t  decode_mem(parse_node_t *head)
{
	data_holder_t holder = {0};
	for(int i = 0; i < head->child_count; ++i)
	{
		parse_node_t *child = head->children[i];
		char *tok = child->tok->lexeme;
		number_type_t type = get_number_type(tok);
		int64_t imm = 0;
		if(type == NUM_INT)
		{
			imm = atoi(tok);
		}
		if(type == NUM_HEX)
		{
			imm = convert_to_hex(tok);
		}
		if(type == NUM_OCT)
		{
			imm = convert_to_oct(tok);
		}

		holder.words_len += imm;
	}
	if(holder.words_len < 0)
	{
		LOG("decode mem data holder size %d is bellow zero\n", holder.words_len);
		exit(EXIT_FAILURE);
	}
	holder.words = CALLOC(holder.words_len, uint64_t);

	return holder;
}

void print_data_holder_string(data_holder_t holder)
{


	char buffer[DEFAULT_BUFFER + 1] = {0};

	size_t size = MIN((DEFAULT_BUFFER / 8), holder.words_len);
	for(size_t c = 0; c < size; ++c)
	{
		char temp[8] = {0};
		int64_to_8chars(holder.words[c], temp);

		for(int bi = 0; bi < 8; ++bi)
		{
			buffer[(c * 8) + bi] = temp[bi];
		}

	}
	printf("\t<%s>\n", buffer);
}



void print_mop_data_string(mop_t *mop)
{
	if(is_mop_id_string(mop->mop))
	{
		print_data_holder_string(mop->data);
	}
}

void print_mop_data_int(mop_t *mop)
{
	if(is_mop_id_integer(mop->mop))
	{

	}
}

void print_mop_data(mop_t *mop)
{
	printf("\tmop: \'%s\'\n", mopids_as_strings[mop->mop]);

	if(is_mop_id_integer(mop->mop))
	{
		print_mop_data_int(mop);
	}
	else if(is_mop_id_string(mop->mop))
	{
		print_mop_data_string(mop);
	}
	else
	{
		switch(mop->mop)
		{
			case MOP_FLOAT:
				//todo print float
				break;
			case MOP_DOUBLE:
				//todo print double
				break;
			case MOP_PTR:
				//todo print ptr
				break;

			case MOP_ALIGN:
				//todo print align
				break;
			case MOP_MEM:
				break;
			default:
				//should never be reached
				LOG("mop id %s %d was not associated with a print", mopids_as_strings[mop->mop], mop->mop, 0);
				exit(EXIT_FAILURE);
				break;
		}
	}
	printf("\t%d\n", mop->data.words_len);
}

void print_mop(mop_t *mop)
{
	printf("mop key: %s id:%d type:%d { \n", mop->mop_keyword, mop->mop, mop->type);
	print_mop_data(mop);
	printf("}\n");
}



mop_t create_mop(parse_node_t *node)
{
	mop_t mop;

	char *mop_tok_id = strdup(node->tok->lexeme);

	mop.mop = get_mop_code(mop_tok_id);
	mop.mop_keyword = mop_tok_id;
	mop.expressions = node->children;
	mop.expressions_len = node->child_count;
	//LOG("create mop\n", 0);

	switch(mop.mop)
	{
		case MOP_STRING:
			mop.data = decode_string(node);
			mop.type = MOP_TYPE_DEFINE_DATA;

			break;
		case MOP_I16:
			mop.data = decode_integer(node);
			mop.type = MOP_TYPE_DEFINE_DATA;

			break;
		case MOP_I32:
			mop.data = decode_integer(node);
			mop.type = MOP_TYPE_DEFINE_DATA;

			break;
		case MOP_I8:
			mop.data = decode_integer(node);
			mop.type = MOP_TYPE_DEFINE_DATA;
			break;
		case MOP_I64:
			mop.data = decode_integer(node);
			mop.type = MOP_TYPE_DEFINE_DATA;

			break;
		case MOP_MEM:
			mop.data = decode_mem(node);
			mop.type = MOP_TYPE_DEFINE_DATA;
			break;
		default:
			LOG("mop not defined %d\n", mop.mop);
			exit(EXIT_FAILURE);
			break;

	}

	return mop;
}

mop_id_t get_mop_code(char *keyword)
{


	mop_id_t ids[] = {
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

	};
	static_assert(ARYSIZE(ids) == ARYSIZE(mopids_as_strings), "array of mop ids string and mopid enum must be the same");
	int code = determine_code(keyword, mopids_as_strings, ARYSIZE(mopids_as_strings));
	if(code != -1)
	{

		return ids[code];
	}
	else
	{
		printf("not a valid mop code %s\n", keyword);
		exit(1);
	}
}