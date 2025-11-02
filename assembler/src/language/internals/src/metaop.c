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

data_holder_t decode_string(parse_node_t *head)
{
	size_t buffer_alloc = 100, buffer_size = 0;
	data_holder_t holder = {0};

	uint8_t *buffer = CALLOC(buffer_alloc, uint8_t);

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

	//printf("buffer: %s\n", buffer);
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


void print_mop(mop_t *mop)
{

}



mop_t create_mop(parse_node_t *node)
{
	mop_t mop;

	char *mop_tok_id = strdup(node->tok->lexeme);

	mop.mop = get_mop_code(mop_tok_id);
	mop.mop_id = mop_tok_id;
	mop.expressions = node->children;
	mop.expressions_len = node->child_count;


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


		default:
			break;

	}

	return mop;
}

mop_id_t get_mop_code(char *keyword)
{
	const char *const mops[] = {
		"i64",
		"i32",
		"i16",
		"i8",
		"u64",
		"u32",
		"u16",
		"u8",
		"float",
		"double",
		"align",
		"string",
		"mem",
		"ptr",

	};

	mop_id_t ids[] = {
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

	int code = determine_code(keyword, mops, ARYSIZE(mops));
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